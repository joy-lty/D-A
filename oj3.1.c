#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXM 4005             // 城市最大数
#define MAXK 240005           // 航班最大数（边数）
#define INF 0x3f3f3f3f

// ------------------------
// 邻接表存反向图
// ------------------------
int head[MAXM];              // head[u] = u 的第一条边
int to[MAXK];                // 边的终点
int cost[MAXK];              // 边的权值
int next_edge[MAXK];         // 下一条边
int edge_cnt = 0;//这几个数组完全不需要

// 插入反向边：存成 to -> from
void add_edge(int u, int v, int w) {
    to[edge_cnt] = v;
    cost[edge_cnt] = w;
    next_edge[edge_cnt] = head[u];
    head[u] = edge_cnt;
    edge_cnt++;
}

// ------------------------
// 简易堆结构（小根堆）
// ------------------------
typedef struct {
    int city;
    int dist;
} Node;

Node heap[MAXK];
int heap_size = 0;

void swap(Node *a, Node *b) {
    Node tmp = *a; *a = *b; *b = tmp;
}

void push(Node x) {
    int i = ++heap_size;
    heap[i] = x;
    while (i > 1 && heap[i].dist < heap[i/2].dist) {
        swap(&heap[i], &heap[i/2]);
        i /= 2;
    }
}

Node pop() {
    Node top = heap[1];
    heap[1] = heap[heap_size--];

    int i = 1;
    while (1) {
        int smallest = i;
        int l = i*2, r = i*2+1;
        if (l <= heap_size && heap[l].dist < heap[smallest].dist) smallest = l;
        if (r <= heap_size && heap[r].dist < heap[smallest].dist) smallest = r;
        if (smallest == i) break;
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return top;
}

// ------------------------
// Dijkstra 单源最短路（基于反向图）
// 输入：start = 终点（如 C 或会场 j）
// 输出：dist[] = 所有城市到 start 的最短费用
// ------------------------
int dist[MAXM];
int visited[MAXM];

void dijkstra(int start) {
    // 初始化
    for (int i = 1; i < MAXM; i++) dist[i] = INF, visited[i] = 0;
    heap_size = 0;

    dist[start] = 0;
    push((Node){start, 0});

    // 普通 Dijkstra
    while (heap_size) {
        Node cur = pop();
        int u = cur.city;

        if (visited[u]) continue;
        visited[u] = 1;

        // 遍历从 u 出发的边（反向图）
        for (int e = head[u]; e != -1; e = next_edge[e]) {
            int v = to[e];
            int w = cost[e];
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                push((Node){v, dist[v]});
            }
        }
    }
}

// ------------------------
// 学者数组
// ------------------------
int scholar[100005];

// ------------------------
// 主程序
// ------------------------
int main() {
    int N, M, K;
    scanf("%d %d %d", &N, &M, &K);

    // 初始化邻接表
    for (int i = 1; i <= M; i++) head[i] = -1;

    // 输入航班并存反向边
    for (int i = 0; i < K; i++) {
        int from, to_city, price;
        scanf("%d %d %d", &from, &to_city, &price);
        add_edge(to_city, from, price);  
        // 原图：from → to
        // 存成反向图：to → from
    }

    // 输入学者出发城市
    for (int i = 1; i <= N; i++) scanf("%d", &scholar[i]);

    // ------------------------
    // 1. 先算所有人到 C(=1) 的费用
    // ------------------------
    dijkstra(1);

    static int dist_toC[MAXM];
    for (int i = 1; i <= M; i++) dist_toC[i] = dist[i];

    // ------------------------
    // 2. 枚举会场 2~M，分别跑 Dijkstra
    // ------------------------
    int best_city = 2;
    long long best_cost = LLONG_MAX;

    for (int j = 2; j <= M; j++) {
        dijkstra(j);

        long long total = 0;
        for (int i = 1; i <= N; i++) {
            int s = scholar[i];
            int c1 = dist_toC[s];  // 学者 s 到 C
            int c2 = dist[s];      // 学者 s 到 j

            total += (c1 < c2 ? c1 : c2);
        }

        // 更新最优答案
        if (total < best_cost || (total == best_cost && j < best_city)) {
            best_cost = total;
            best_city = j;
        }
    }

    // ------------------------
    // 输出
    // ------------------------
    printf("%d\n%lld\n", best_city, best_cost);
    return 0;
}