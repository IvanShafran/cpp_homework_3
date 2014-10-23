#include <iostream>
#include <vector>
#include <string>
#include <map>


class AKAutomat
{
    struct AKVertex
    {
        bool leaf;
        std::map<unsigned char, int> child;
        int parent;
        unsigned char last_edge;
        int suffix_link;
        std::map<unsigned char, int> automat_link;
        int distanceFromRoot;
        int maxSuffixLength;

        AKVertex() : leaf(false), parent(0), suffix_link(-1), distanceFromRoot(0), maxSuffixLength(-1)
        { }

        AKVertex(int parent_, int distance, unsigned char symbol) : leaf(false), parent(parent_), last_edge(symbol),
            suffix_link(-1), distanceFromRoot(distance), maxSuffixLength(-1)
        {}
    };

    bool isKeyExistInMap(int key, const std::map<unsigned char, int>& map) const
    {
        return map.find(key) != map.end();
    }

    std::vector<AKVertex> vertices;

public:
    AKAutomat() : vertices(1)
    {}

    const std::vector<AKVertex>& getVertices() const {
        return vertices;
    }

    int size() const
    {
        return vertices.size();
    }

    void addString(const std::string& string)
    {
        int vertex = 0;//root
        for (size_t index = 0; index < string.size(); ++index)
        {
            unsigned char symbol = string[index];

            if (!isKeyExistInMap(symbol, vertices[vertex].child))
            {
                vertices.push_back(AKVertex(vertex, index + 1, symbol));
                vertices[vertex].child[symbol] = vertices.size() - 1;
            }
            vertex = vertices[vertex].child[symbol];
        }
        vertices[vertex].leaf = true;
    }

    int getSuffixLink(int vertex)
    {
        if (vertices[vertex].suffix_link == -1)
        {
            if (vertex == 0 || vertices[vertex].parent == 0)
                vertices[vertex].suffix_link = 0;
            else
                vertices[vertex].suffix_link = getAutomatLink(getSuffixLink(vertices[vertex].parent),
                vertices[vertex].last_edge);
        }

        return vertices[vertex].suffix_link;
    }

    int getAutomatLink(int vertex, unsigned char symbol)
    {
        if (!isKeyExistInMap(symbol, vertices[vertex].automat_link))
        {
            if (isKeyExistInMap(symbol, vertices[vertex].child))
                vertices[vertex].automat_link[symbol] = vertices[vertex].child[symbol];
            else {
                if (vertex == 0)
                    vertices[vertex].automat_link[symbol] = 0;
                else
                    vertices[vertex].automat_link[symbol] = getAutomatLink(getSuffixLink(vertex), symbol);
            }
        }

        return vertices[vertex].automat_link[symbol];
    }

    int getMaxSuffixLengthLeaf(int vertex)
    {
        if (vertices[vertex].maxSuffixLength == -1)
        {
            if (vertex == 0)
                vertices[vertex].maxSuffixLength = 0;
            else
            {
                if (vertices[vertex].leaf)
                {
                    vertices[vertex].maxSuffixLength = vertices[vertex].distanceFromRoot;
                }
                else
                {
                    vertices[vertex].maxSuffixLength = getMaxSuffixLengthLeaf(getSuffixLink(vertex));
                }
            }
        }

        return vertices[vertex].maxSuffixLength;
    }
};

bool processLine(int numberOfLine, const std::string& line, AKAutomat& automat)
{
    int vertex = 0;
    for (size_t index = 0; index < line.size(); ++index)
    {
        unsigned char symbol = line[index];
        vertex = automat.getAutomatLink(vertex, symbol);

        if (automat.getMaxSuffixLengthLeaf(vertex) != 0)
        {
            std::cout << numberOfLine << ' ' << index + 2 - automat.getMaxSuffixLengthLeaf(vertex);
            return true;
        }
    }

    return false;
}

int main()
{
    AKAutomat automat;

    int n;
    std::cin >> n;
    std::string line;
    std::getline(std::cin, line);
    for (size_t index = 0; index < n; ++index)
    {
        std::string string;
        std::getline(std::cin, string);
        automat.addString(string);
    }

    int m;
    std::cin >> m;
    std::getline(std::cin, line);
    for (size_t index = 0; index < m; ++index)
    {
        std::string line;
        std::getline(std::cin, line);
        if (processLine(index + 1, line, automat)) {
            return 0;
        }
    }
    std::cout << "Passed";
}
