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

void writeAnswer(const std::string& pattern, const std::vector<size_t>& listOfPrefixesLength)
{
    std::cout << "No" << std::endl;
    for (size_t index = 0; index < listOfPrefixesLength.size(); ++index)
    {
        std::cout << pattern.substr(0, listOfPrefixesLength[index]) << ' ';
    }
}

zBlock getMostRightZBlock(size_t start_position, size_t finish_position, const std::vector<size_t>& zFunction)
{
    zBlock mostRightZBlock;
    for (size_t index = start_position; index <= finish_position; ++index)
    if (mostRightZBlock.right < zBlock::getRightBound(index, zFunction))
    {
        mostRightZBlock.left = index;
        mostRightZBlock.right = zBlock::getRightBound(index, zFunction);
    }

    return mostRightZBlock;
}

void solution1684()
{
    std::string pattern, lastJackWord;
    std::cin >> pattern >> lastJackWord;

    std::vector<size_t> zFunction = getZFunction(pattern + "$" + lastJackWord, 
        [](char a, char b){ return a == b; });

    std::vector<size_t> listOfPrefixesLength;
    zBlock zBlock_(pattern.size() + 1);
    zBlock_.right = zBlock::getRightBound(zBlock_.left, zFunction);

    while (zBlock_.right + 1 < zFunction.size())
    {
        zBlock mostRightZBlock = getMostRightZBlock(zBlock_.left, zBlock_.right + 1, zFunction);

        if (mostRightZBlock == zBlock_) 
        {
            std::cout << "Yes" << std::endl;
            return;
        }

        listOfPrefixesLength.push_back(mostRightZBlock.left - zBlock_.left);
        zBlock_.left = mostRightZBlock;
    }
    listOfPrefixesLength.push_back(zFunction[zBlock_.left]);

    writeAnswer(pattern, listOfPrefixesLength);
}

int main()
{
    solution1684();
}
