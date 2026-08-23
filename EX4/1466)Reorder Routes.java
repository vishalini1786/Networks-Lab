import java.util.*;

class Solution {
    public int minReorder(int n, int[][] connections) {

        ArrayList<int[]>[] graph = new ArrayList[n];

        for (int i = 0; i < n; i++) {
            graph[i] = new ArrayList<>();
        }

        for (int[] connection : connections) {
            int a = connection[0];
            int b = connection[1];

            // Original direction: a -> b
            graph[a].add(new int[]{b, 1});

            // Reverse direction for traversal: b -> a
            graph[b].add(new int[]{a, 0});
        }

        boolean[] visited = new boolean[n];

        return dfs(0, graph, visited);
    }

    private int dfs(int city, ArrayList<int[]>[] graph, boolean[] visited) {

        visited[city] = true;

        int count = 0;

        for (int[] edge : graph[city]) {

            int nextCity = edge[0];
            int cost = edge[1];

            if (!visited[nextCity]) {
                count += cost;
                count += dfs(nextCity, graph, visited);
            }
        }

        return count;
    }
}
