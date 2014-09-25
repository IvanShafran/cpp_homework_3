#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

struct zBlock
{
    size_t left, right;

    zBlock(size_t left_ = 0, size_t right_ = 0)
    {
        left = left_;
        right = right_;
    }

    static size_t getRightBound(size_t leftBound, const std::vector<size_t>& zFunction)
    {
        return leftBound + zFunction[leftBound] - 1;
    }

    bool operator==(const zBlock& anotherZBlock)
    {
        return left == anotherZBlock.left;
    }
};

template <class Sequence, class Pred>
std::vector<size_t> getZFunction(const Sequence& sequence, Pred equal)
{
    size_t sizeOfSequence = sequence.size();
    std::vector<size_t> zFunction;

    zBlock mostRightZBlock;

    zFunction.push_back(sizeOfSequence);
    for (size_t index = 1; index < sizeOfSequence; ++index)
    {
        zFunction.push_back(0);
        if (mostRightZBlock.left != 0 && index <= mostRightZBlock.right)
        {
            zFunction[index] = std::min(mostRightZBlock.right - index + 1, zFunction[index - mostRightZBlock.left]);
        }

        while ((index + zFunction[index] < sizeOfSequence) &&
            equal(sequence[zFunction[index]], sequence[index + zFunction[index]]))
        {
            ++zFunction[index];
        }

        if (zBlock::getRightBound(index, zFunction) > mostRightZBlock.right)
        {
            mostRightZBlock.left = index;
            mostRightZBlock.right = zBlock::getRightBound(index, zFunction);
        }
    }

    return zFunction;
}

void solution1354()
{
    std::string string, reversed_string;
    std::cin >> string;
    if (string.size() == 1)
    {
        std::cout << string + string;
        return;
    }

    reversed_string = string;
    std::reverse(reversed_string.begin(), reversed_string.end());

    std::vector<size_t> zFunction = getZFunction(reversed_string + '$' + string, [](char a, char b){ return a == b; });

    for (size_t index = string.size() + 2; index < zFunction.size(); ++index)
    if (zBlock::getRightBound(index, zFunction) == zFunction.size() - 1)
    {
        std::string additive;
        additive = string.substr(0, index - string.size() - 1);
        std::reverse(additive.begin(), additive.end());

        std::cout << string << additive;
        return;
    }
}

int main()
{
    solution1354();
}
