#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <limits>
#include <memory>

using namespace std;

/**
 * Advanced C++ STL and Algorithm Implementations
 */
class AdvancedCPP {
public:
    // 1. Custom Smart Pointer (simplified)
    template<typename T>
    class SmartPointer {
    private:
        T* ptr;
        int* ref_count;
        
        void cleanup() {
            if (--(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
        }
        
    public:
        explicit SmartPointer(T* p = nullptr) : ptr(p), ref_count(new int(1)) {}
        
        SmartPointer(const SmartPointer<T>& other) : ptr(other.ptr), ref_count(other.ref_count) {
            ++(*ref_count);
        }
        
        SmartPointer<T>& operator=(const SmartPointer<T>& other) {
            if (this != &other) {
                cleanup();
                ptr = other.ptr;
                ref_count = other.ref_count;
                ++(*ref_count);
            }
            return *this;
        }
        
        ~SmartPointer() { cleanup(); }
        
        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }
        T* get() const { return ptr; }
    };

    // 2. Custom Vector Implementation
    template<typename T>
    class CustomVector {
    private:
        T* data;
        size_t capacity;
        size_t size;
        
        void resize() {
            capacity = capacity == 0 ? 1 : capacity * 2;
            T* new_data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
        
    public:
        CustomVector() : data(nullptr), capacity(0), size(0) {}
        
        ~CustomVector() { delete[] data; }
        
        void push_back(const T& value) {
            if (size >= capacity) resize();
            data[size++] = value;
        }
        
        void pop_back() {
            if (size > 0) --size;
        }
        
        T& operator[](size_t index) { return data[index]; }
        const T& operator[](size_t index) const { return data[index]; }
        
        size_t getSize() const { return size; }
        size_t getCapacity() const { return capacity; }
        
        void clear() { size = 0; }
    };

    // 3. Graph Implementation with Modern C++
    class Graph {
    private:
        unordered_map<int, vector<pair<int, int>>> adj; // node -> list of (neighbor, weight)
        
    public:
        void addEdge(int u, int v, int weight = 1) {
            adj[u].emplace_back(v, weight);
            adj[v].emplace_back(u, weight); // undirected
        }
        
        // Dijkstra's Algorithm using modern C++ features
        unordered_map<int, int> dijkstra(int start) {
            unordered_map<int, int> distances;
            for (const auto& node : adj) {
                distances[node.first] = numeric_limits<int>::max();
            }
            distances[start] = 0;
            
            // Min-heap using priority_queue with custom comparator
            auto comp = [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.second > b.second;
            };
            priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)> pq(comp);
            pq.emplace(start, 0);
            
            while (!pq.empty()) {
                auto [current, current_dist] = pq.top();
                pq.pop();
                
                if (current_dist > distances[current]) continue;
                
                for (const auto& [neighbor, weight] : adj[current]) {
                    int new_dist = current_dist + weight;
                    if (new_dist < distances[neighbor]) {
                        distances[neighbor] = new_dist;
                        pq.emplace(neighbor, new_dist);
                    }
                }
            }
            return distances;
        }
        
        // BFS with lambda visitor
        void bfs(int start, function<void(int)> visit = nullptr) {
            unordered_set<int> visited;
            queue<int> q;
            q.push(start);
            visited.insert(start);
            
            while (!q.empty()) {
                int current = q.front();
                q.pop();
                
                if (visit) visit(current);
                
                for (const auto& [neighbor, _] : adj[current]) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push(neighbor);
                    }
                }
            }
        }
    };

    // 4. Advanced Sorting Algorithms
    class Sorting {
    public:
        // Merge Sort with iterators (STL style)
        template<typename RandomIt>
        static void mergeSort(RandomIt first, RandomIt last) {
            if (first >= last - 1) return;
            
            auto mid = first + (last - first) / 2;
            mergeSort(first, mid);
            mergeSort(mid, last);
            merge(first, mid, last);
        }
        
        // Quick Sort with custom comparator
        template<typename RandomIt, typename Compare>
        static void quickSort(RandomIt first, RandomIt last, Compare comp) {
            if (first >= last) return;
            
            auto pivot = partition(first, last, comp);
            quickSort(first, pivot, comp);
            quickSort(pivot + 1, last, comp);
        }
        
    private:
        template<typename RandomIt>
        static void merge(RandomIt first, RandomIt mid, RandomIt last) {
            vector<typename iterator_traits<RandomIt>::value_type> temp;
            temp.reserve(distance(first, last));
            
            auto left = first, right = mid;
            while (left != mid && right != last) {
                if (*left <= *right) {
                    temp.push_back(*left++);
                } else {
                    temp.push_back(*right++);
                }
            }
            
            temp.insert(temp.end(), left, mid);
            temp.insert(temp.end(), right, last);
            copy(temp.begin(), temp.end(), first);
        }
        
        template<typename RandomIt, typename Compare>
        static RandomIt partition(RandomIt first, RandomIt last, Compare comp) {
            auto pivot = last - 1;
            auto i = first;
            
            for (auto j = first; j < pivot; ++j) {
                if (comp(*j, *pivot)) {
                    swap(*i++, *j);
                }
            }
            swap(*i, *pivot);
            return i;
        }
    };

    // 5. Modern C++ String Algorithms
    class StringAlgorithms {
    public:
        // KMP Algorithm with modern C++
        static vector<int> kmpSearch(const string& text, const string& pattern) {
            vector<int> lps = computeLPS(pattern);
            vector<int> matches;
            int i = 0, j = 0;
            
            while (i < text.length()) {
                if (pattern[j] == text[i]) {
                    ++i; ++j;
                }
                if (j == pattern.length()) {
                    matches.push_back(i - j);
                    j = lps[j - 1];
                } else if (i < text.length() && pattern[j] != text[i]) {
                    if (j != 0) j = lps[j - 1];
                    else ++i;
                }
            }
            return matches;
        }
        
        // Longest Common Subsequence using DP
        static string longestCommonSubsequence(const string& s1, const string& s2) {
            int m = s1.length(), n = s2.length();
            vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
            
            for (int i = 1; i <= m; ++i) {
                for (int j = 1; j <= n; ++j) {
                    if (s1[i - 1] == s2[j - 1]) {
                        dp[i][j] = dp[i - 1][j - 1] + 1;
                    } else {
                        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                    }
                }
            }
            
            // Reconstruct LCS
            string lcs;
            int i = m, j = n;
            while (i > 0 && j > 0) {
                if (s1[i - 1] == s2[j - 1]) {
                    lcs = s1[i - 1] + lcs;
                    --i; --j;
                } else if (dp[i - 1][j] > dp[i][j - 1]) {
                    --i;
                } else {
                    --j;
                }
            }
            return lcs;
        }
        
    private:
        static vector<int> computeLPS(const string& pattern) {
            vector<int> lps(pattern.length(), 0);
            int len = 0, i = 1;
            
            while (i < pattern.length()) {
                if (pattern[i] == pattern[len]) {
                    lps[i++] = ++len;
                } else {
                    if (len != 0) len = lps[len - 1];
                    else lps[i++] = 0;
                }
            }
            return lps;
        }
    };
};

// Demonstration and Testing
int main() {
    cout << "=== Advanced C++ STL and Algorithms ===" << endl;
    
    // Test Custom Vector
    AdvancedCPP::CustomVector<int> vec;
    for (int i = 0; i < 10; ++i) vec.push_back(i * 2);
    cout << "Custom Vector: ";
    for (size_t i = 0; i < vec.getSize(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
    
    // Test Smart Pointer
    AdvancedCPP::SmartPointer<int> sp1(new int(42));
    AdvancedCPP::SmartPointer<int> sp2 = sp1;
    cout << "Smart Pointer value: " << *sp1 << endl;
    
    // Test Graph Algorithms
    AdvancedCPP::Graph g;
    g.addEdge(0, 1, 4); g.addEdge(0, 2, 1);
    g.addEdge(2, 1, 2); g.addEdge(2, 3, 5);
    g.addEdge(1, 3, 1); g.addEdge(3, 4, 3);
    
    auto distances = g.dijkstra(0);
    cout << "Dijkstra from node 0: ";
    for (const auto& [node, dist] : distances) {
        cout << "Node " << node << ": " << dist << " | ";
    }
    cout << endl;
    
    // Test Sorting
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    AdvancedCPP::Sorting::mergeSort(arr.begin(), arr.end());
    cout << "Merge Sorted: ";
    for (int num : arr) cout << num << " ";
    cout << endl;
    
    // Test String Algorithms
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    auto matches = AdvancedCPP::StringAlgorithms::kmpSearch(text, pattern);
    cout << "KMP Matches at indices: ";
    for (int match : matches) cout << match << " ";
    cout << endl;
    
    string s1 = "ABCDGH", s2 = "AEDFHR";
    string lcs = AdvancedCPP::StringAlgorithms::longestCommonSubsequence(s1, s2);
    cout << "LCS of '" << s1 << "' and '" << s2 << "': " << lcs << endl;
    
    return 0;
}