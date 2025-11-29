#include <stdio.h>
#include <stdlib.h>
#define MAXN 100010
#define MAXM 4010
#define MAXK 240010
#define INF 0x3f3f3f3f

//邻接表+最小堆适合稀疏图
typedef struct {
    short to;
    unsigned char price;
} Edge;
Edge *edges;
int edge_count = 0;
int *head;//每个节点的第一条边
int *next;//后继边
void add_edge(int u, int v, int w) {
    edges[edge_count].to = v;
    edges[edge_count].price = w;
    next[edge_count] = head[u];
    head[u] = edge_count++;
}
//最小堆
typedef struct {
    int node;
    int dist;
} HeapNode;
HeapNode *heap;
int heap_size = 0;
void up(int i) {
    while (i > 1) {
        int p = i >> 1;
        if (heap[p].dist <= heap[i].dist) break;
        HeapNode tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}
void down(int i) {
    while (1) {
        int l = i << 1, r = l + 1, min_i = i;
        if (l <= heap_size && heap[l].dist < heap[min_i].dist) min_i = l;
        if (r <= heap_size && heap[r].dist < heap[min_i].dist) min_i = r;
        if (min_i == i) break;
        HeapNode tmp = heap[min_i];
        heap[min_i] = heap[i];
        heap[i] = tmp;
        i = min_i;
    }
}
void push(int node, int dist) {
    heap[++heap_size].node = node;
    heap[heap_size].dist = dist;
    up(heap_size);
}
HeapNode pop() {
    HeapNode top = heap[1];
    heap[1] = heap[heap_size--];
    down(1);
    return top;
}
int *dist;
int *visited;
void dijkstra(int start, int M) {
    for (int i = 1; i <= M; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    heap_size = 0;
    dist[start] = 0;
    push(start, 0);
    while (heap_size > 0) {
        HeapNode h = pop();
        int u = h.node;
        if (visited[u]) continue;
        visited[u] = 1;
        for (int i = head[u]; i != -1; i = next[i]) {
            int v = edges[i].to;
            int w = edges[i].price;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                push(v, dist[v]);
            }
        }
    }
}
//邻接矩阵适合稠密图仅限M较小时
unsigned char **Matrix = NULL;
int distM[500];
unsigned char visitedM[500];
void dijkstraM(int start, int MC) {
    for (int i = 1; i <= MC; i++) {
        distM[i] = INF;
        visitedM[i] = 0;
    }
    distM[start] = 0;
    for (int i = 1; i <= MC; i++) {
        int u = -1, min = INF;
        for (int j = 1; j <= MC; j++) {
            if (!visitedM[j] && distM[j] < min) {
                min = distM[j];
                u = j;
            }
        }
        if (u == -1) break;
        visitedM[u] = 1;
        for (int v = 1; v <= MC; v++) {
            if (!visitedM[v] && Matrix[u][v] != 255) {
                int t = distM[u] + Matrix[u][v];
                if (t < distM[v]) distM[v] = t;
            }
        }
    }
}

int main() {
    int N, M, K;
    scanf("%d %d %d", &N, &M, &K);
    int key = (M *M/ 5 > K) ? 1 : 0;//判断稀疏图还是稠密图
    int city[MAXM] = {0};
    int dist_toC[MAXM];
    //均使用动态分配，避免两边同时分配内存
    if (!key) {
        Matrix = (unsigned char **)malloc(M * sizeof(unsigned char *));
        for (int i = 1; i <= M; i++) {
            Matrix[i] = (unsigned char *)malloc(M * sizeof(unsigned char));
            for (int j = 1; j <= M; j++) Matrix[i][j] = 255;
        }
        for (int i = 0; i < K; i++) {
            int from, to, price;
            scanf("%d %d %d", &from, &to, &price);
            Matrix[to][from] = price;
        }
    } else {
        edges = (Edge *)malloc((K + 5) * sizeof(Edge));
        next = (int *)malloc((K + 5) * sizeof(int));
        head = (int *)malloc((M + 5) * sizeof(int));
        for (int i = 1; i <= M; i++) head[i] = -1;
        heap = (HeapNode *)malloc((M * 5 + 5) * sizeof(HeapNode));
        dist = (int *)malloc((M + 5) * sizeof(int));
        visited = (int *)malloc((M + 5) * sizeof(int));
        for (int i = 0; i < K; i++) {
            int from, to, price;
            scanf("%d %d %d", &from, &to, &price);
            add_edge(to, from, price);
        }
    }
    for (int i = 1; i <= N; i++) {
        int t;
        scanf("%d", &t);
        city[t]++;
    }
    if (!key) {
        dijkstraM(1, M);
        for (int i = 1; i <= M; i++) dist_toC[i] = distM[i];
    } else {
        dijkstra(1, M);
        for (int i = 1; i <= M; i++) dist_toC[i] = dist[i];
    }
    int best_city = -1;
    unsigned long long best_cost = INF;
    for (int i = 2; i <= M; i++) {
        if (!key) {
            if (distM[i] == INF && city[i] == 0) continue;
            dijkstraM(i, M);
        } else {
            if (dist[i] == INF && city[i] == 0) continue;
            dijkstra(i, M);
        }
        unsigned long long total_cost = 0;
        for (int j = 1; j <= M; j++) {
            if (city[j] == 0) continue;
            unsigned int d = (!key ? distM[j] : dist[j]);
            d = (d < dist_toC[j] ? d : dist_toC[j]);
            total_cost += (unsigned long long)city[j] * d;
            if (total_cost >= best_cost) break;
        }
        if (total_cost < best_cost) {
            best_city = i;
            best_cost = total_cost;
        }
    }
    printf("%d\n%llu\n", best_city, best_cost);
    if (!key) {
        for (int i = 0; i <= M; i++) free(Matrix[i]);
        free(Matrix);
    } else {
        free(edges);    free(next);    free(head);
        free(heap);    free(dist);    free(visited);
    }
    return 0;
}