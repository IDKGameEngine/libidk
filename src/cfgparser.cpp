#include "libidk/cfgparser.hpp"
#include <cctype>
#include <cstdio>
#include <utility>

using namespace idk;

CfgParser::CfgParser() : rootNode_(new TreeNode("__root__")) {}

CfgParser::CfgParser(const char *path) : CfgParser()
{
    load(path);
}

CfgParser::~CfgParser()
{
    delete rootNode_;
}

CfgParser::TreeNode::~TreeNode()
{
    for (auto &[key, node] : nodes_)
    {
        if (node->isLeaf())
            delete static_cast<TreeLeaf *>(node);
        else
            delete static_cast<TreeNode *>(node);
    }
}

bool CfgParser::load(const char *path)
{
    mError = nullptr;
    std::FILE *file = std::fopen(path, "rb");
    if (!file)
    {
        mError = "Could not open configuration file";
        return false;
    }
    // The parser already owns dynamic tree/string storage. Bound file size and
    // nesting so an editable configuration cannot consume unbounded resources.
    if (std::fseek(file, 0, SEEK_END) != 0)
    {
        std::fclose(file);
        mError = "Could not seek configuration file";
        return false;
    }
    long length = std::ftell(file);
    if (length < 0 || length > 1024 * 1024 || std::fseek(file, 0, SEEK_SET) != 0)
    {
        std::fclose(file);
        mError = "Invalid configuration file size (maximum 1 MiB)";
        return false;
    }
    CfgParser candidate;
    candidate.mBuf.resize(size_t(length));
    bool read = !length || std::fread(candidate.mBuf.data(), 1, size_t(length), file) == size_t(length);
    bool closed = std::fclose(file) == 0;
    if (!read || !closed)
    {
        mError = "Could not read configuration file";
        return false;
    }
    for (char ch : candidate.mBuf)
    {
        if (!ch)
        {
            mError = "Unexpected null byte in configuration";
            return false;
        }
    }
    if (!candidate.parse(candidate.rootNode_, false, 0))
    {
        mError = candidate.mError;
        return false;
    }
    std::swap(rootNode_, candidate.rootNode_);
    return true;
}

char CfgParser::peek()
{
    return mIdx < mBuf.size() ? mBuf[mIdx] : '\0';
}

char CfgParser::advance()
{
    char ch = peek();
    if (ch)
        ++mIdx;
    return ch;
}

void CfgParser::skipWhitespace()
{
    for (;;)
    {
        if (std::isspace(static_cast<unsigned char>(peek())))
            advance();
        else if (peek() == '#')
        {
            while (peek() && advance() != '\n') {}
        }
        else
            return;
    }
}

bool CfgParser::parse(TreeNode *current, bool section, unsigned depth)
{
    if (depth > 32)
    {
        mError = "Configuration nesting exceeds 32 sections";
        return false;
    }
    for (;;)
    {
        skipWhitespace();
        if (!peek())
        {
            if (section)
                mError = "Missing closing section brace";
            return !section;
        }
        if (peek() == '}' && section)
        {
            advance();
            return true;
        }
        if (peek() == '[')
        {
            advance();
            std::string name;
            while (peek() && peek() != ']' && peek() != '\n' && peek() != '\r' &&
                   name.size() < MAX_NODE_NAME_LENGTH)
                name.push_back(advance());
            if (advance() != ']' || name.empty() || current->find(name.c_str()))
            {
                mError = "Invalid or duplicate section name";
                return false;
            }
            skipWhitespace();
            if (advance() != '{')
            {
                mError = "Expected opening section brace";
                return false;
            }
            auto *node = current->insert<TreeNode>(name, name);
            if (!parse(node, true, depth + 1))
                return false;
            continue;
        }
        if (!section || (!std::isalpha(static_cast<unsigned char>(peek())) && peek() != '_'))
        {
            mError = "Expected section or setting";
            return false;
        }
        std::string key;
        while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_' || peek() == '-' || peek() == '.')
        {
            if (key.size() >= MAX_NODE_NAME_LENGTH)
            {
                mError = "Setting name is too long";
                return false;
            }
            key.push_back(advance());
        }
        while (peek() == ' ' || peek() == '\t')
            advance();
        if (advance() != '=' || current->find(key.c_str()))
        {
            mError = "Expected '=' or found duplicate setting";
            return false;
        }
        while (peek() == ' ' || peek() == '\t')
            advance();
        std::string value;
        if (peek() == '"')
        {
            advance();
            while (peek() && peek() != '"' && peek() != '\n' && peek() != '\r')
                value.push_back(advance());
            if (advance() != '"')
            {
                mError = "Unterminated quoted value";
                return false;
            }
        }
        else
        {
            while (peek() && peek() != '\n' && peek() != '\r' && peek() != '}' && peek() != '#')
                value.push_back(advance());
            while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())))
                value.pop_back();
        }
        while (peek() == ' ' || peek() == '\t')
            advance();
        if (peek() && peek() != '\n' && peek() != '\r' && peek() != '}' && peek() != '#')
        {
            mError = "Unexpected text after setting";
            return false;
        }
        current->insert<TreeLeaf>(key, value);
    }
}

const CfgParser::TreeBase *CfgParser::TreeNode::find(const char *searchKey) const
{
    for (const auto &[key, node] : nodes_)
        if (key == searchKey)
            return node;
    return nullptr;
}

const CfgParser::TreeBase *CfgParser::find(const char *key) const
{
    return rootNode_->find(key);
}

const CfgParser::TreeNode &CfgParser::operator[](const char *key) const
{
    return (*rootNode_)[key];
}

const CfgParser::TreeNode &CfgParser::TreeNode::operator[](const char *key) const
{
    static TreeLeaf dummy("");
    // Preserve the legacy leaf-as-node access API. New code can use find()
    // and check the tag before casting to TreeNode or TreeLeaf.
    const TreeBase *node = isLeaf() ? nullptr : find(key);
    return *reinterpret_cast<const TreeNode *>(node ? node : &dummy);
}

void CfgParser::print() { _print(rootNode_); }

void CfgParser::_print(TreeNode *current)
{
    SysLog log("%s", current->mName.c_str());
    for (auto &[key, node] : *current)
    {
        if (node->isLeaf())
            log("%s = \"%s\"", key.c_str(), static_cast<TreeLeaf *>(node)->getValue());
        else
            _print(static_cast<TreeNode *>(node));
    }
}
