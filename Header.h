#include <algorithm>
#include <fstream>
#include <istream>
#include <map>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <iostream>

class DSU {
private:
	std::vector<int> p;
	std::vector<int> r;

public:
	DSU(int n)
		: p(n + 1), r(n + 1)
	{
		for (int i = 0; i <= n; ++i) {
			make(i);
		}
	}

	void make(int v) {
		p[v] = v;
		r[v] = 0;
	}

	int find(int v) {
		if (v == p[v]) {
			return v;
		}
		return p[v] = find(p[v]);
	}

	void join(int a, int b) {
		a = find(a);
		b = find(b);
		if (a != b) {
			if (r[a] < r[b]) {
				p[a] = b;
			}
			else {
				p[b] = a;
			}
			if (r[a] == r[b]) {
				++r[a];
			}
		}
	}
};

class IGraph {
public:
	virtual ~IGraph() = default;

	virtual void readGraph(std::istream& stream) = 0;
	virtual void writeGraph(std::ostream& stream) = 0;

	virtual void addEdge(int from, int to, int weight) = 0;
	virtual void removeEdge(int from, int to) = 0;
	virtual int  changeEdge(int from, int to, int weight) = 0;
	virtual int  getEdge(int from, int to) = 0;

	virtual IGraph* transformToAdjMatrix() = 0;
	virtual IGraph* transformToAdjList() = 0;
	virtual IGraph* transformToListOfEdges() = 0;

	virtual std::vector<std::vector<int>>& getAdjMatrix() {
		throw 0;
	}

	virtual std::vector<std::map<int, int>>& getAdjList() {
		throw 0;
	}

	virtual std::vector<std::tuple<int, int, int>>& getListOfEdges() {
		throw 0;
	}

	virtual IGraph* getCopy() = 0;
	virtual void    getProperties(int& n, int& r, int& w) = 0;
	virtual int     getSize() = 0;
};

class AdjMatrixGraph : public IGraph {
private:
	std::vector<std::vector<int>> data;
	int n;
	int r;
	int w;

public:
	AdjMatrixGraph(int n = 0, int r = 0, int w = 0)
		: data(n, std::vector<int>(n, 0)), n(n), r(r), w(w)
	{
	}

	void readGraph(std::istream& stream) override;
	void writeGraph(std::ostream& stream) override;

	void addEdge(int from, int to, int weight) override;
	void removeEdge(int from, int to) override;
	int  changeEdge(int from, int to, int weight) override;
	int  getEdge(int from, int to) override;

	IGraph* transformToAdjMatrix() override;
	IGraph* transformToAdjList() override;
	IGraph* transformToListOfEdges() override;

	std::vector<std::vector<int>>& getAdjMatrix() override;

	IGraph* getCopy() override;
	void    getProperties(int& n, int& r, int& w) override;
	int     getSize() override;
};

class AdjListGraph : public IGraph {
private:
	std::vector<std::map<int, int>> data;
	int n;
	int r;
	int w;

public:
	AdjListGraph(int n = 0, int r = 0, int w = 0)
		: data(n), n(n), r(r), w(w)
	{
	}

	void readGraph(std::istream& stream) override;
	void writeGraph(std::ostream& stream) override;

	void addEdge(int from, int to, int weight) override;
	void removeEdge(int from, int to) override;
	int  changeEdge(int from, int to, int weight) override;
	int  getEdge(int from, int to) override;

	IGraph* transformToAdjMatrix() override;
	IGraph* transformToAdjList() override;
	IGraph* transformToListOfEdges() override;

	std::vector<std::map<int, int>>& getAdjList() override;

	IGraph* getCopy() override;
	void    getProperties(int& n, int& r, int& w) override;
	int     getSize() override;
};

class ListOfEdgesGraph : public IGraph {
public:
	struct EdgeComparator {
		bool operator()(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
			int aa;
			int ba;
			int wa;
			int ab;
			int bb;
			int wb;
			std::tie(aa, ba, wa) = a;
			std::tie(ab, bb, wb) = b;
			if (wa < wb) {
				return true;
			}
			else if (wa == wb) {
				if (aa < ab) {
					return true;
				}
				else if (aa == ab) {
					if (ba < bb) {
						return true;
					}
				}
			}
			return false;
		}
	};

private:
	std::vector<std::tuple<int, int, int>> data;
	int n;
	int m;
	int r;
	int w;

public:
	ListOfEdgesGraph(int n = 0, int m = 0, int r = 0, int w = 0)
		: n(n), m(m), r(r), w(w)
	{
	}

	void readGraph(std::istream& stream) override;
	void writeGraph(std::ostream& stream) override;

	void addEdge(int from, int to, int weight) override;
	void removeEdge(int from, int to) override;
	int  changeEdge(int from, int to, int weight) override;
	int  getEdge(int from, int to) override;

	IGraph* transformToAdjMatrix() override;
	IGraph* transformToAdjList() override;
	IGraph* transformToListOfEdges() override;

	std::vector<std::tuple<int, int, int>>& getListOfEdges() override;

	IGraph* getCopy() override;
	void    getProperties(int& n, int& r, int& w) override;
	int     getSize() override;
};

void AdjMatrixGraph::readGraph(std::istream& stream) {
	stream >> n;
	stream >> r >> w;
	std::vector<std::vector<int>>(n, std::vector<int>(n, 0)).swap(data);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			stream >> data[i][j];
		}
	}
}

void AdjMatrixGraph::writeGraph(std::ostream& stream) {
	stream << "C" << " " << n << "\n";
	stream << r << " " << w << "\n";
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			stream << data[i][j] << ((j < n - 1) ? " " : "\n");
		}
	}
}

void AdjMatrixGraph::addEdge(int from, int to, int weight) {
	if (!r)
		weight = 1;
	data[from - 1][to - 1] = weight;
	if (r == 0) {
		data[to - 1][from - 1] = weight;
	}
}

void AdjMatrixGraph::removeEdge(int from, int to) {
	data[from - 1][to - 1] = 0;
	if (r == 0) {
		data[to - 1][from - 1] = 0;
	}
}

int AdjMatrixGraph::changeEdge(int from, int to, int weight) {
	auto temp = data[from - 1][to - 1];
	data[from - 1][to - 1] = weight;
	if (r == 0) {
		data[to - 1][from - 1] = weight;
	}
	return temp;
}

int AdjMatrixGraph::getEdge(int from, int to) {
	return (data[from - 1][to - 1] != 0) ? data[from - 1][to - 1] : -1;
}

IGraph* AdjMatrixGraph::transformToAdjMatrix() {
	return this;
}

IGraph* AdjMatrixGraph::transformToAdjList() {
	auto temp = new AdjListGraph(n, r, w);
	for (int i = 0; i < n; ++i) {
		for (int j = ((r == 1) ? 0 : i); j < n; ++j) {
			if (data[i][j] != 0) {
				temp->addEdge(i + 1, j + 1, data[i][j]);
			}
		}
	}
	return temp;
}

IGraph* AdjMatrixGraph::transformToListOfEdges() {
	auto temp = new ListOfEdgesGraph(n, 0, r, w);
	for (int i = 0; i < n; ++i) {
		for (int j = ((r == 1) ? 0 : i); j < n; ++j) {
			if (data[i][j] != 0) {
				temp->addEdge(i + 1, j + 1, data[i][j]);
			}
		}
	}
	return temp;
}

std::vector<std::vector<int>>& AdjMatrixGraph::getAdjMatrix() {
	return data;
}

IGraph* AdjMatrixGraph::getCopy() {
	return new AdjMatrixGraph(*this);
}

void AdjMatrixGraph::getProperties(int& n, int& r, int& w) {
	n = this->n;
	r = this->r;
	w = this->w;
}

int AdjMatrixGraph::getSize() {
	return n;
}

void AdjListGraph::readGraph(std::istream& stream) {
	stream >> n;
	stream >> r >> w;
	std::vector<std::map<int, int>>(n).swap(data);
	std::string line;
	std::getline(stream, line);
	for (int i = 0; i < n; ++i) {
		std::getline(stream, line);
		std::istringstream lineStream(line);
		int bi = 0;
		int wi = 0;
		while (lineStream >> bi) {
			if (w == 1) {
				lineStream >> wi;
			}
			data[i].emplace(bi, wi);
		}
	}
}

void AdjListGraph::writeGraph(std::ostream& stream) {
	stream << "L" << " " << n << "\n";
	stream << r << " " << w << "\n";
	for (int i = 0; i < n; ++i) {
		std::ostringstream lineStream;
		for (auto& item : data[i]) {
			lineStream << item.first << " ";
			if (w == 1) {
				lineStream << item.second << " ";
			}
		}
		auto line = lineStream.str();
		if (!line.empty()) {
			line.erase(line.end() - 1);
		}
		stream << line << "\n";
	}
}

void AdjListGraph::addEdge(int from, int to, int weight) {
	data[from - 1][to] = weight;
	if (r == 0) {
		data[to - 1][from] = weight;
	}
}

void AdjListGraph::removeEdge(int from, int to) {
	data[from - 1].erase(to);
	if (r == 0) {
		data[to - 1].erase(from);
	}
}

int AdjListGraph::changeEdge(int from, int to, int weight) {
	auto temp = data[from - 1][to];
	data[from - 1][to] = weight;
	if (r == 0) {
		data[to - 1][from] = weight;
	}
	return temp;
}

int AdjListGraph::getEdge(int from, int to) {
	auto item = data[from - 1].find(to);
	return (item != data[from - 1].end()) ? item->second : -1;
}

IGraph* AdjListGraph::transformToAdjMatrix() {
	auto temp = new AdjMatrixGraph(n, r, w);
	for (int i = 0; i < n; ++i) {
		for (auto& item : data[i]) {
			if ((r == 0) && (i + 1 > item.first)) {
				continue;
			}
			temp->addEdge(i + 1, item.first, item.second);
		}
	}
	return temp;
}

IGraph* AdjListGraph::transformToAdjList() {
	return this;
}

IGraph* AdjListGraph::transformToListOfEdges() {
	auto temp = new ListOfEdgesGraph(n, 0, r, w);
	for (int i = 0; i < n; ++i) {
		for (auto& item : data[i]) {
			if ((r == 0) && (i + 1 > item.first)) {
				continue;
			}
			temp->addEdge(i + 1, item.first, item.second);
		}
	}
	return temp;
}

std::vector<std::map<int, int>>& AdjListGraph::getAdjList() {
	return data;
}

IGraph* AdjListGraph::getCopy() {
	return new AdjListGraph(*this);
}

void AdjListGraph::getProperties(int& n, int& r, int& w) {
	n = this->n;
	r = this->r;
	w = this->w;
}

int AdjListGraph::getSize() {
	return n;
}

void ListOfEdgesGraph::readGraph(std::istream& stream) {
	stream >> n >> m;
	stream >> r >> w;
	for (int i = 0; i < m; ++i) {
		int ai = 0;
		int bi = 0;
		int wi = 0;
		stream >> ai >> bi;
		if (w == 1) {
			stream >> wi;
		}
		if ((r == 0) && (ai > bi)) {
			std::swap(ai, bi);
		}
		data.emplace_back(ai, bi, wi);
	}
}

void ListOfEdgesGraph::writeGraph(std::ostream& stream) {
	stream << "E" << " " << n << " " << m << "\n";
	stream << r << " " << w << "\n";
	for (int i = 0; i < m; ++i) {
		int ai;
		int bi;
		int wi;
		std::tie(ai, bi, wi) = data[i];
		stream << ai << " " << bi;
		if (w == 1) {
			stream << " " << wi;
		}
		stream << "\n";
	}
}

void ListOfEdgesGraph::addEdge(int from, int to, int weight) {
	if ((r == 0) && (from > to)) {
		std::swap(from, to);
	}
	data.emplace_back(from, to, weight);
	++m;
}

void ListOfEdgesGraph::removeEdge(int from, int to) {
	if ((r == 0) && (from > to)) {
		std::swap(from, to);
	}
	auto edge = std::find_if(data.begin(), data.end(), [from, to](std::tuple<int, int, int>& edge) {
		return (std::get<0>(edge) == from) && (std::get<1>(edge) == to);
	});
	data.erase(edge);
	--m;
}

int ListOfEdgesGraph::changeEdge(int from, int to, int weight) {
	if ((r == 0) && (from > to)) {
		std::swap(from, to);
	}
	auto edge = std::find_if(data.begin(), data.end(), [from, to](std::tuple<int, int, int>& edge) {
		return (std::get<0>(edge) == from) && (std::get<1>(edge) == to);
	});
	std::swap(std::get<2>(*edge), weight);
	return weight;
}

int ListOfEdgesGraph::getEdge(int from, int to) {
	if ((r == 0) && (from > to)) {
		std::swap(from, to);
	}
	auto edge = std::find_if(data.begin(), data.end(), [from, to](std::tuple<int, int, int>& edge) {
		return (std::get<0>(edge) == from) && (std::get<1>(edge) == to);
	});
	return (edge != data.end()) ? std::get<2>(*edge) : -1;
}

IGraph* ListOfEdgesGraph::transformToAdjMatrix() {
	auto temp = new AdjMatrixGraph(n, r, w);
	for (auto& edge : data) {
		int ai;
		int bi;
		int wi;
		std::tie(ai, bi, wi) = edge;
		temp->addEdge(ai, bi, wi);
	}
	return temp;
}

IGraph* ListOfEdgesGraph::transformToAdjList() {
	auto temp = new AdjListGraph(n, r, w);
	for (auto& edge : data) {
		int ai;
		int bi;
		int wi;
		std::tie(ai, bi, wi) = edge;
		temp->addEdge(ai, bi, wi);
	}
	return temp;
}

IGraph* ListOfEdgesGraph::transformToListOfEdges() {
	return this;
}

std::vector<std::tuple<int, int, int>>& ListOfEdgesGraph::getListOfEdges() {
	return data;
}

IGraph* ListOfEdgesGraph::getCopy() {
	return new ListOfEdgesGraph(*this);
}

void ListOfEdgesGraph::getProperties(int& n, int& r, int& w) {
	n = this->n;
	r = this->r;
	w = this->w;
}

int ListOfEdgesGraph::getSize() {
	return n;
}

class Graph {
private:
	struct QueueComparator {
		bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) {
			int av;
			int aw;
			int bv;
			int bw;
			std::tie(av, aw) = a;
			std::tie(bv, bw) = b;
			if ((aw == -1) && (bw != -1)) {
				return true;
			}
			if ((aw != -1) && (bw == -1)) {
				return false;
			}

			if (aw > bw) {
				return true;
			}
			if (aw == bw) {
				return av > bv;
			}
			return false;
		}
	};

private:
	IGraph* graph;
	char type;

public:
	Graph(char type, int n, int r, int w) {
		this->type = type;
		switch (type) {
		case 'C':
			graph = new AdjMatrixGraph(n, r, w);
			break;
		case 'L':
			graph = new AdjListGraph(n, r, w);
			break;
		case 'E':
			graph = new ListOfEdgesGraph(n, 0, r, w);
			break;
		}
	}

	Graph(const Graph& other)
		: graph(other.graph->getCopy())
	{
		this->type = other.type;
	}

	Graph()
		: graph(nullptr)
	{
	}

	~Graph() {
		if (graph != nullptr) {
			delete graph;
		}
	}

	void readGraph(std::string fileName) {
		std::ifstream file(fileName);
		switch (file.get()) {
		case 'C':
			graph = new AdjMatrixGraph();
			type = 'C';
			break;
		case 'L':
			graph = new AdjListGraph();
			type = 'L';
			break;
		case 'E':
			graph = new ListOfEdgesGraph();
			type = 'E';
			break;
		}
		graph->readGraph(file);
	}

	void writeGraph(std::string fileName) {
		std::ofstream file(fileName);
		graph->writeGraph(file);
	}

	void addEdge(int from, int to, int weight) {
		graph->addEdge(from, to, weight);
	}

	void removeEdge(int from, int to) {
		graph->removeEdge(from, to);
	}

	int changeEdge(int from, int to, int weight) {
		return graph->changeEdge(from, to, weight);
	}

	void transformToAdjMatrix() {
		auto temp = graph->transformToAdjMatrix();
		type = 'C';
		if (temp != graph) {
			delete graph;
			graph = temp;
		}
	}

	void transformToAdjList() {
		type = 'L';
		auto temp = graph->transformToAdjList();
		if (temp != graph) {
			delete graph;
			graph = temp;
		}
	}

	void transformToListOfEdges() {
		auto temp = graph->transformToListOfEdges();
		type = 'E';
		if (temp != graph) {
			delete graph;
			graph = temp;
		}
	}

	Graph getSpanningTreeKruscal() {
		transformToListOfEdges();
		auto& edges = graph->getListOfEdges();
		std::sort(edges.begin(), edges.end(), ListOfEdgesGraph::EdgeComparator());
		int n;
		int r;
		int w;
		graph->getProperties(n, r, w);
		DSU dsu(n);
		Graph result('E', n, r, w);
		int a;
		int b;
		int c;
		for (auto& edge : edges) {
			std::tie(a, b, c) = edge;
			if (dsu.find(a) != dsu.find(b)) {
				dsu.join(a, b);
				result.addEdge(a, b, c);
			}
		}
		return result;
	}

	Graph getSpanningTreePrima() {
		transformToAdjList();
		int n;
		int r;
		int w;
		graph->getProperties(n, r, w);
		Graph result('E', n, r, w);

		std::vector<bool> used(n, false);
		std::vector<int> key(n, -1);
		std::vector<int> parent(n, -1);
		std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, QueueComparator> queue;

		key[0] = 0;
		for (int i = 0; i < n; ++i) {
			queue.emplace(i, key[i]);
		}

		while (!queue.empty()) {
			int min = queue.top().first;
			key[min] = queue.top().second;
			queue.pop();

			if (used[min]) {
				continue;
			}
			used[min] = true;

			if (parent[min] != -1) {
				result.addEdge(min + 1, parent[min] + 1, graph->getEdge(min + 1, parent[min] + 1));
			}

			for (auto& edge : graph->getAdjList()[min]) {
				int v = edge.first - 1;
				int w = edge.second;
				if ((!used[v]) && ((w < key[v]) || (key[v] == -1))) {
					key[v] = w;
					parent[v] = min;
					queue.emplace(v, key[v]);
				}
			}
		}

		return result;
	}

	Graph getSpanningTreeBoruvka() {
		transformToListOfEdges();
		int n;
		int r;
		int w;
		graph->getProperties(n, r, w);
		DSU dsu(n);
		Graph result('E', n, r, w);

		auto& edges = graph->getListOfEdges();
		int m = edges.size();
		int k = n;

		while (k > 1) {
			std::vector<int> min(n, -1);
			bool isForest = true;
			for (int i = 0; i < m; ++i) {
				int from = dsu.find(std::get<0>(edges[i])) - 1;
				int to = dsu.find(std::get<1>(edges[i])) - 1;
				int weight = std::get<2>(edges[i]);
				if (from != to) {
					if ((min[from] == -1) || (weight < std::get<2>(edges[min[from]]))) {
						min[from] = i;
						isForest = false;
					}
					if ((min[to] == -1) || (weight < std::get<2>(edges[min[to]]))) {
						min[to] = i;
						isForest = false;
					}
				}
			}

			if (isForest) {
				break;
			}

			for (int i = 0; i < n; ++i) {
				if (min[i] != -1) {
					int from = std::get<0>(edges[min[i]]);
					int to = std::get<1>(edges[min[i]]);
					int weight = std::get<2>(edges[min[i]]);
					if (dsu.find(from) != dsu.find(to)) {
						result.addEdge(from, to, weight);
						dsu.join(from, to);
						--k;
					}
				}
			}
		}

		return result;
	}

	int checkEuler(bool& circleExist) {
		circleExist = false;
		transformToAdjList();
		int oddDegree = 0;
		int startVert = 0;
		auto& adjList = graph->getAdjList();

		int n = graph->getSize();

		std::vector<int> edgesNumber(n + 1, 0);
		std::vector<int> parentsNumber(n + 1, 0);

		for (int i = 0; i < int(adjList.size()); ++i) {
			if ((adjList[i].size() % 2) != 0) {
				++oddDegree;
				startVert = i + 1;
			}
			edgesNumber[i + 1] = adjList[i].size();
		}

		if (oddDegree > 2) {
			return 0;
		}

		transformToListOfEdges();
		DSU dsu(n);
		for (auto& edge : graph->getListOfEdges()) {
			int a;
			int b;
			int c;
			std::tie(a, b, c) = edge;
			dsu.join(a, b);
		}

		int num = 0;
		for (int i = 1; i < n; ++i) {
			if (dsu.find(i) != dsu.find(i + 1)) {
				parentsNumber[dsu.find(i)] = 1;
				parentsNumber[dsu.find(i + 1)] = 1;
				//return 0;
			}
		}

		for (int i = 1; i <= n; i++) {
			if (parentsNumber[i] == 1 && edgesNumber[i] > 0)
				num++;
			if (num > 1) {
				circleExist = false;
				return 0;
			}
		}

		if (startVert == 0) {
			circleExist = true;
			startVert = 1;
		}
		return startVert;
	}

	bool isBridge(int a, int b) {
		transformToAdjList();
		int n, r, w;
		graph->getProperties(n, r, w);
		//Graph gr(*this);
		//gr.removeEdge(a, b);
		//removeEdge(a, b);
		std::queue<int> q;
		q.push(a);
		std::vector<bool> used(n + 1);
		std::vector<int> d(n + 1, 0);
		used[a] = true;
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			for (auto& j = /*gr.*/graph->getAdjList()[v - 1].begin(); j != /*gr.*/graph->getAdjList()[v - 1].end(); j++) {
				int to = j->first;
				if (v == a && to == b) {
					continue;
				}
				if (!used[to]) {
					used[to] = true;
					q.push(to);
					//d[to] = d[v] + 1;
					if (to == b) {
						return false;
					}
				}
			}
		}
		//addEdge(a, b)
		//return !d[b];
		return true;
	}

	std::vector<int> getEuleranTourFleri() {
		std::vector<int> res;
		bool circleExist;
		int start = checkEuler(circleExist);
		if (start == 0)
			return res;
		transformToAdjList();
		Graph gr(*this);
		int currV = start;
		res.push_back(currV);
		while (true) {
			int deg = gr.graph->getAdjList()[currV - 1].size();
			if (deg == 0)
				break;
			bool fl = true;
			for (auto& i : gr.graph->getAdjList()[currV - 1]) {
				int nextV = i.first;
				if ((deg == 1) || !gr.isBridge(currV, nextV)) {
					fl = false;
					gr.removeEdge(currV, nextV);
					currV = nextV;
					res.push_back(currV);
					break;
				}
			}
			if (fl) {
				int nextV = gr.graph->getAdjList()[currV - 1].begin()->first;
				gr.removeEdge(currV, nextV);
				currV = nextV;
				res.push_back(currV);
			}
		}
		return res;
	}

	std::vector<int> getEuleranTourEffective() {
		std::vector<int> result;
		bool circleExist;
		int start = checkEuler(circleExist);
		if (start == 0) {
			return result;
		}

		transformToAdjList();
		Graph g(*this);
		auto& adjList = g.graph->getAdjList();

		std::stack<int> stack;
		stack.push(start);
		while (!stack.empty()) {
			int currV = stack.top();
			for (auto& edge : adjList[currV - 1]) {
				int nextV = edge.first;
				stack.push(nextV);
				g.removeEdge(currV, nextV);
				break;
			}
			if (currV == stack.top()) {
				stack.pop();
				result.push_back(currV);
			}
		}
		return result;
	}

	bool checkBipart(std::vector<char>& marks) {
		transformToAdjList();
		int n = graph->getAdjList().size();
		std::queue<int> q;
		if (n < 2)
			return false;
		for (int vert = 1; vert <= n; vert++) {
			if (marks[vert] != 'A' && marks[vert] != 'B') {
				marks[vert] = 'A';
				q.push(vert);
				//int counter = 1;
				while (!q.empty()) {
					int prevV = q.front();
					q.pop();
					for (auto& i : graph->getAdjList()[prevV - 1]) {
						if (marks[i.first] != 'A' && marks[i.first] != 'B') {
							marks[i.first] = marks[prevV] == 'A' ? 'B' : 'A';
							q.push(i.first);
							//counter++;
						}
						else {
							if (marks[i.first] == marks[prevV]) {
								return false;
							}
						}
					}
				}
			}
		}
		return true;
	}


	std::vector<std::pair<int, int>> getMaximumMatchingBipart() {
		transformToAdjList();
		int n = graph->getAdjList().size();
		std::vector<std::pair<int, int>> res;
		std::vector<int> currBipart(n + 1, -1);
		std::vector<char> marks(n + 1, 'Z');
		std::vector<bool> used(n + 1, false);

		std::vector<bool> used1(n + 1);
		for (int i = 1; i <= n; i++) {
			if (marks[i] == 'B')
				continue;
			for (auto& j : graph->getAdjList()[i - 1]) {
				if (currBipart[j.first] == -1) {
					currBipart[j.first] = i;
					used1[i] = true;
					break;
				}
			}
		}

		if (checkBipart(marks)) {
			for (int i = 1; i <= n; i++) {
				if (used1[i] || marks[i] == 'B')
					continue;
				used.assign(n + 1, false);
				//kuhnDFS(i, used, currBipart);
			}
			for (int i = 1; i <= n; i++) {
				if (marks[i] == 'B' && currBipart[i] != -1)
					res.emplace_back(currBipart[i], i);
			}

		}
		return res;
	}

	struct Edge {
		int from, to, next, flow, cap;
		Edge() {}
		Edge(int from, int to, int next, int f, int c) : from(from), to(to), next(next), flow(f), cap(c)
		{}
	};

	int dfsPush(int source, int sink, int flow, std::vector<Edge> &edges, std::vector<int> &head, std::vector<bool> &used) {
		if (source == sink)
			return flow;
		used[source] = true;
		for (int i = head[source]; i != -1; i = edges[i].next)
			if (!used[edges[i].to] && edges[i].flow < edges[i].cap) {
				int pushed = dfsPush(edges[i].to, sink, std::min(flow, edges[i].cap - edges[i].flow), edges, head, used);
				if (pushed) {
					edges[i].flow += pushed;
					edges[i ^ 1].flow -= pushed;
					return pushed;
				}
			}
		return 0;
	}
};
