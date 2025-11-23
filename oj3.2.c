#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXM 4005
#define MAXN 100005
#define INF 1000000000

typedef struct {
    int to, cost;
    int next;
} Edge;

Edge edges[500005];
int head[MAXM];
int edgeCount = 0;

int N, M, K;
int scholars[MAXN];

void addEdge(int from, int to, int cost) {
    edges[edgeCount].to = to;
    edges[edgeCount].cost = cost;
    edges[edgeCount].next = head[from];
    head[from] = edgeCount;
    edgeCount++;
}

void dijkstra(int src, int dist[]) {
    int visited[MAXM];
    memset(visited, 0, sizeof(visited));
    
    for (int i = 1; i <= M; i++) dist[i] = INF;
    dist[src] = 0;
    
    for (int iter = 0; iter < M; iter++) {
        int u = -1, minDist = INF;
        for (int i = 1; i <= M; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        if (u == -1 || minDist == INF) break;
        visited[u] = 1;
        
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            int cost = edges[e].cost;
            if (dist[u] + cost < dist[v]) {
                dist[v] = dist[u] + cost;
            }
        }
    }
}

int main() {
    scanf("%d %d %d", &N, &M, &K);
    
    memset(head, -1, sizeof(head));
    
    // 建立反向图
    for (int i = 0; i < K; i++) {
        int from, to, price;
        scanf("%d %d %d", &from, &to, &price);
        addEdge(to, from, price);  // 反向添加边
    }
    
    for (int i = 0; i < N; i++) {
        scanf("%d", &scholars[i]);
    }
    
    // 计算从城市1到所有城市的距离（在反向图上）
    int dist_from_c1[MAXM];
    dijkstra(1, dist_from_c1);
    
    long long minCost = LLONG_MAX;
    int bestCity = 2;
    
    // 尝试每个可能的分会场城市
    for (int city = 2; city <= M; city++) {
        // 计算从该分会场城市到所有城市的距离（在反向图上）
        int dist_from_city[MAXM];
        dijkstra(city, dist_from_city);
        
        long long totalCost = 0;
        int feasible = 1;
        
        for (int i = 0; i < N; i++) {
            int scholar_start = scholars[i];
            int dist_to_c = dist_from_c1[scholar_start];
            int dist_to_branch = dist_from_city[scholar_start];
            
            if (dist_to_c == INF && dist_to_branch == INF) {
                feasible = 0;
                break;
            }
            
            int cost;
            if (dist_to_c == INF) cost = dist_to_branch;
            else if (dist_to_branch == INF) cost = dist_to_c;
            else cost = (dist_to_c < dist_to_branch) ? dist_to_c : dist_to_branch;
            
            totalCost += cost;
        }
        
        if (feasible && totalCost < minCost) {
            minCost = totalCost;
            bestCity = city;
        }
    }
    
    printf("%d\n%lld\n", bestCity, minCost);
    
    return 0;
}