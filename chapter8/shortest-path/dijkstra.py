from __future__ import annotations

from dataclasses import dataclass
from heapq import heappop, heappush
from math import inf


@dataclass
class DirectedEdge:
    from_vertex: int
    to_vertex: int
    weight: float

    def __str__(self) -> str:
        return f"{self.from_vertex}->{self.to_vertex}({self.weight:.2f})"


class EdgeWeightedDigraph:
    def __init__(self, vertex_count: int) -> None:
        self.V = vertex_count
        self.E = 0
        self.adj: list[list[DirectedEdge]] = [[] for _ in range(vertex_count)]

    def add_edge(self, edge: DirectedEdge) -> None:
        self.adj[edge.from_vertex].append(edge)
        self.E += 1

    def adjacent(self, vertex: int) -> list[DirectedEdge]:
        return self.adj[vertex]

    def print_adj(self) -> None:
        for vertex in range(self.V):
            edges = " ".join(str(edge) for edge in self.adj[vertex])
            print(f"{vertex}: {edges}".rstrip())


class DijkstraSP:
    def __init__(self, graph: EdgeWeightedDigraph, source: int) -> None:
        self.source = source
        self.dist_to = [inf] * graph.V
        self.edge_to: list[DirectedEdge | None] = [None] * graph.V
        self.visited = [False] * graph.V
        self.dist_to[source] = 0.0

        pq: list[tuple[float, int]] = []
        heappush(pq, (0.0, source))

        while pq:
            _, vertex = heappop(pq)
            if self.visited[vertex]:
                continue

            self.visited[vertex] = True

            for edge in graph.adjacent(vertex):
                if self.visited[edge.to_vertex]:
                    continue

                self._relax(edge, pq)

    def _relax(self, edge: DirectedEdge, pq: list[tuple[float, int]]) -> None:
        v = edge.from_vertex
        w = edge.to_vertex
        candidate = self.dist_to[v] + edge.weight

        if candidate < self.dist_to[w]:
            self.dist_to[w] = candidate
            self.edge_to[w] = edge
            heappush(pq, (self.dist_to[w], w))

    def has_path_to(self, vertex: int) -> bool:
        return self.dist_to[vertex] < inf

    def path_to(self, vertex: int) -> list[DirectedEdge]:
        if not self.has_path_to(vertex):
            return []

        path: list[DirectedEdge] = []
        edge = self.edge_to[vertex]
        while edge is not None:
            path.append(edge)
            edge = self.edge_to[edge.from_vertex]

        path.reverse()
        return path


def build_demo_graph() -> EdgeWeightedDigraph:
    graph = EdgeWeightedDigraph(8)

    graph.add_edge(DirectedEdge(4, 5, 0.35))
    graph.add_edge(DirectedEdge(5, 4, 0.35))
    graph.add_edge(DirectedEdge(4, 7, 0.37))
    graph.add_edge(DirectedEdge(5, 7, 0.28))
    graph.add_edge(DirectedEdge(7, 5, 0.28))
    graph.add_edge(DirectedEdge(5, 1, 0.32))
    graph.add_edge(DirectedEdge(0, 4, 0.38))
    graph.add_edge(DirectedEdge(0, 2, 0.26))
    graph.add_edge(DirectedEdge(7, 3, 0.39))
    graph.add_edge(DirectedEdge(1, 3, 0.29))
    graph.add_edge(DirectedEdge(2, 7, 0.34))
    graph.add_edge(DirectedEdge(6, 2, 0.40))
    graph.add_edge(DirectedEdge(3, 6, 0.52))
    graph.add_edge(DirectedEdge(6, 0, 0.58))
    graph.add_edge(DirectedEdge(6, 4, 0.93))

    return graph


def main() -> None:
    graph = build_demo_graph()
    source = 0
    shortest_paths = DijkstraSP(graph, source)

    print("Adjacency lists:")
    graph.print_adj()
    print()

    print(f"Dijkstra from {source}:")
    for vertex in range(graph.V):
        if not shortest_paths.has_path_to(vertex):
            print(f"{source} to {vertex}: unreachable")
            continue

        path = " ".join(str(edge) for edge in shortest_paths.path_to(vertex))
        print(f"{source} to {vertex} ({shortest_paths.dist_to[vertex]:.2f}): {path}")

    print()
    print("dist_to:", [round(value, 2) for value in shortest_paths.dist_to])
    print(
        "edge_to:",
        [str(edge) if edge is not None else None for edge in shortest_paths.edge_to],
    )


if __name__ == "__main__":
    main()
