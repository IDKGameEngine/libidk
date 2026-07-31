#pragma once

#include <map>
#include <string>
#include <vector>

#include "libidk/assert.hpp"
#include "libidk/log.hpp"

namespace idk
{
    class CfgParser;
}


class idk::CfgParser
{
public:
    static constexpr size_t MAX_NODE_NAME_LENGTH = 128;

    class TreeBase;
    class TreeLeaf;
    class TreeNode;

    CfgParser();
    void load(const char *path);
    void print();
    const TreeNode &operator[](const char *key) const;

private:
    TreeNode *rootNode_;
    std::vector<char> mBuf;
    size_t mIdx, mLine, mCol;

    std::string _readSection();
    std::string _readLabel();
    std::string _readTo(char);
    std::string _readString();
    std::string _readValue();

    void skip(char);
    char peek();
    char advance();
    char retreat();
    char match(char);

    void _parse(TreeNode*);
    void _parse_section(TreeNode*);
    void _print(TreeNode*);

};




class idk::CfgParser::TreeBase
{
private:
    const bool isLeaf_;

public:
    TreeBase(bool leaf): isLeaf_(leaf) {  }
    bool isLeaf() const { return isLeaf_; }
};


class idk::CfgParser::TreeLeaf: public TreeBase
{
public:
    std::string mValue;

    TreeLeaf(const std::string &value)
    :   TreeBase(true), mValue(value)
    {

    }

    const char *getValue() const
    {
        return mValue.c_str();
    }
};


class idk::CfgParser::TreeNode: public TreeBase
{
private:
    std::map<std::string, TreeBase*> nodes_;

public:
    std::string mName;

    TreeNode(const std::string &name)
    :   TreeBase(false), mName(name)
    {

    }

    const char *getValue() const
    {
        return reinterpret_cast<const TreeLeaf*>(this)->getValue();
    }

    const char *toStr() const { return getValue(); }
    int32_t toI32() const { return atoi(getValue()); }
    uint16_t toU16() const { return atol(getValue()); }
    uint64_t toU64() const { return atoll(getValue()); }
    double toF64() const { return atof(getValue()); }

    const TreeNode &operator[](const char *key) const;

    template <typename T, typename... Args>
    T *insert(const std::string &key, Args... args)
    {
        IDK_ASSERT(!nodes_.contains(key), "nodes_ already contains \"{}\"", key);

        T *ndptr = new T(args...);
        nodes_[key] = static_cast<TreeBase*>(ndptr);
        return ndptr;
    }

    auto begin() { return nodes_.begin(); }
    auto end()   { return nodes_.end(); }
    const auto begin() const { return nodes_.begin(); }
    const auto end()   const { return nodes_.end(); }
};

