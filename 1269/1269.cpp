struct AKvertex
{
    bool leaf;
    std::map<unsigned char, int> child;
    int parent;
    unsigned char last_edge;
    int suffix_link;
    std::map<unsigned char, int> automat_link;
    int distanceFromRoot;
    int lengthOfMostLongSuffix;

    AKvertex() : leaf(false), parent(0), suffix_link(-1), distanceFromRoot(0), lengthOfMostLongSuffix(-1)
    { }
};

class AKAutomat
{
public:
    bool isKeyExistInMap(int key, const std::map<unsigned char, int>& map)
    {
        return map.find(key) != map.end();
    }

    int size()
    {
        return vertices.size();
    }

    std::vector<AKvertex> vertices;

    AKAutomat() : vertices(1)
    {}

    void addString(const std::string& string)
    {
        int vertex = 0;//root
        for (size_t index = 0; index < string.size(); ++index)
        {
            unsigned char symbol = static_cast<unsigned char>(string[index]);

            if (!isKeyExistInMap(symbol, vertices[vertex].child))
            {
                AKvertex newAKVertex;
                newAKVertex.parent = vertex;
                newAKVertex.last_edge = symbol;
                newAKVertex.distanceFromRoot = index + 1;
                vertices.push_back(newAKVertex);
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

    int getLengthOfMostLongSuffixLeaf(int vertex)
    {
        if (vertices[vertex].lengthOfMostLongSuffix == -1)
        {
            if (vertex == 0)
                vertices[vertex].lengthOfMostLongSuffix = 0;
            else
            {
                if (vertices[vertex].leaf)
                {
                    vertices[vertex].lengthOfMostLongSuffix = vertices[vertex].distanceFromRoot;
                }
                else
                {
                    vertices[vertex].lengthOfMostLongSuffix = getLengthOfMostLongSuffixLeaf(getSuffixLink(vertex));
                }
            }
        }

        return vertices[vertex].lengthOfMostLongSuffix;
    }

    int getLengthOfMostLongSuffixLeaf(int vertex)
    {
        if (vertices[vertex].lengthOfMostLongSuffix == -1)
        {
            if (vertex == 0)
                vertices[vertex].lengthOfMostLongSuffix = 0;
            else
            {
                if (vertices[vertex].leaf)
                {
                    vertices[vertex].lengthOfMostLongSuffix = vertices[vertex].distanceFromRoot;
                }
                else
                {
                    vertices[vertex].lengthOfMostLongSuffix = getLengthOfMostLongSuffixLeaf(getSuffixLink(vertex));
                }
            }
        }

        return vertices[vertex].lengthOfMostLongSuffix;
    }
};

bool processLine(int numberOfLine, const std::string& line, AKAutomat& automat)
{
    int vertex = 0;
    for (size_t index = 0; index < line.size(); ++index)
    {
        unsigned char symbol = static_cast<unsigned char>(line[index]);
        vertex = automat.getAutomatLink(vertex, symbol);

        if (automat.getLengthOfMostLongSuffixLeaf(vertex) != 0)
        {
            std::cout << numberOfLine << ' ' << index + 2 - automat.getLengthOfMostLongSuffixLeaf(vertex);
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
