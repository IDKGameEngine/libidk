#include "libidk/cfgparser.hpp"
#include "libidk/file.hpp"
#include "libidk/log.hpp"
#include <cstring>

using namespace idk;


void CfgParser::skip(char)
{
    char ch = mBuf[mIdx];

    while (mIdx < mBuf.size() && ch)
    {
        ch = mBuf[mIdx++];
    }

    if (mIdx >= mBuf.size() - 1)
    {
        mIdx = mBuf.size() - 1;
    }
}


char CfgParser::peek()
{
    if (mIdx >= mBuf.size() - 1)
    {
        mIdx = mBuf.size() - 1;
        return '\0';
    }
    return mBuf[mIdx];
}


char CfgParser::advance()
{
    char ch = peek();
    if (!ch) return ch;

    mIdx += 1;
    mCol += 1;

    if (ch == '\n')
    {
        mLine += 1;
        mCol = 1;
    }

    return ch;
}


char CfgParser::retreat()
{
    mIdx -= 1;
    return peek();
}


char CfgParser::match( char ch )
{
    if (peek() == ch)
        return advance();
    return '\0';    
}


std::string CfgParser::_readLabel()
{
    std::string str = "";
    while (char ch = advance())
    {
        if (ch=='=' || (!isalnum(ch) && ch!='_' && ch!='-' && ch!='.'))
            break;
        str.push_back(ch);
    }
    return str;
}


std::string CfgParser::_readTo(char stop)
{
    std::string str = "";
    while (char ch = advance())
    {
        if (ch==stop) break;
        str.push_back(ch);
    }
    return str;
}


CfgParser::CfgParser()
:   rootNode_(new TreeNode("__root__"))
{

}

CfgParser::CfgParser(const char *path)
:   CfgParser()
{
    this->load(path);
}

void CfgParser::load(const char *path)
{
    mBuf.clear();
    mIdx = 0;
    mLine = 0;
    mCol = 0;

    idk::FileReader reader(path);

    auto  *src = static_cast<const char*>(reader.getData());
    auto  *end = src + reader.getSize();

    while ((src < end) && *src)
    {
        char ch = *(src++);
        if (ch == ' ')
            continue;
        mBuf.push_back(ch);
    }

    mIdx  = 0;
    mLine = 1;
    mCol  = 1;

    _parse(rootNode_);
}


void CfgParser::_parse(TreeNode *curr)
{
    while (char ch = peek())
    {
        if (match('\n'))
        {
            continue;
        }

        else if (match('['))
        {
            std::string sname = _readTo(']');
            _parse_section(curr->insert<TreeNode>(sname, sname));
        }
    
        else if (isalpha(ch))
        {
            IDK_ASSERT(false, "Invalid format");
        }
    }
}

void CfgParser::_parse_section(TreeNode *curr)
{
    _readTo('{'); // [SectionName] { ...

    while (char ch = peek())
    {
        if (match('\n'))
        {
            continue;
        }

        else if (match('}'))
        {
            return;
        }

        else if (match('['))
        {
            std::string sname = _readTo(']');
            _parse_section(curr->insert<TreeNode>(sname, sname));
        }

        else if (isalpha(ch))
        {
            std::string key = _readLabel();

            if (match('\"'))
            {
                std::string value = _readTo('\"');
                curr->insert<TreeLeaf>(key, value);
            }
            else
            {
                std::string value = _readTo('\n');
                curr->insert<TreeLeaf>(key, value);
            }
        }
    }
}


const CfgParser::TreeNode &CfgParser::operator[](const char *searchKey) const
{
    return (*rootNode_)[searchKey];
}

const CfgParser::TreeNode &CfgParser::TreeNode::operator[](const char *searchKey) const
{
    static TreeLeaf dummy("");

    const TreeNode &curr = *this;
    for (const auto &[key, nd]: curr)
    {
        if (key != std::string(searchKey))
        {
            continue;
        }
        if (nd->isLeaf())
        {
            return *reinterpret_cast<TreeNode*>(nd);
        }
        else
        {
            return *reinterpret_cast<TreeNode*>(nd);
        }
    }

    return *reinterpret_cast<TreeNode*>(&dummy);
}


void CfgParser::print() { _print(rootNode_); }
void CfgParser::_print(CfgParser::TreeNode *currNode)
{
    TreeNode &curr = *currNode;
    SysLog log("%s", curr.mName.c_str());

    for (auto &[key, nd]: curr)
    {
        if (nd->isLeaf())
        {
            log("%s = \"%s\"", key.c_str(), ((TreeLeaf*)nd)->mValue.c_str());
        }
        else
        {
            _print((TreeNode*)nd);
        }
    }
}



// {
//     for (auto &[section, keyval]: mData)
//     {
//         idk::SysLog log("Section %s", section.c_str());

//         for (auto &[key, value]: keyval)
//         {
//             log("%s = \"%s\"", key.c_str(), value.c_str());
//         }
//     }
// }





// TreeBase *operator[](const std::string &key)
// {
//     if (nodes_.contains(key))
//     {
//         return nodes_[key];
//     }
// }


