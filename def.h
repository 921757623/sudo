/*
 * @Description: 定义库，包含库函数、define以及数据结构
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:19
 * @LastEditTime: 2022-09-06 09:19:29
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <io.h>
#include <direct.h>

typedef int status;
typedef int boolean;
typedef int KeyType;

#define ERROR 0
#define OK 1
#define true 1
#define false 0
#define LITERAL_SIZE 10
#define MAX_EXECUTE_TIME 20
#define MAX_VERTEX_NUM 100
#define BASE_LITERAL_NUM 1458
#define BASE_CLAUSE_NUM 18792

typedef struct LiteralNode
{
    int data;
    struct LiteralNode *next;
} LiteralNode, *LiteralList;

typedef struct ClauseNode
{
    int nodeNum; //统计文字个数
    struct LiteralNode *head;
    struct ClauseNode *next;
} ClauseNode, *ClauseList;

typedef struct parameter
{
    ClauseList clause;
    boolean value[3000];
    int literalNum;
    pthread_t *thread;
} parameter, *param;

struct node
{
    boolean isAssign;
    int value;
};

int datasource[21][55] = {
    {42, 1116, 1187, 1218, 1243, 1252, 1296, 1365, 1392, 1422, 1448, 1471, 1545, 1567, 1614, 1663, 1699, 1717, 1733, 1819, 1934, 1958, 1981, 2141, 2155, 2173, 2266, 2288, 2321, 2374, 2442, 2471, 2495, 2549, 2568, 2584, 2646, 2669, 2692, 2712, 2725, 2814, 2886},
    {46, 1118, 1174, 1211, 1235, 1247, 1256, 1279, 1282, 1353, 1424, 1455, 1473, 1536, 1619, 1688, 1742, 1754, 1831, 1846, 1859, 1863, 1912, 1961, 1995, 2144, 2271, 2335, 2397, 2424, 2452, 2463, 2532, 2561, 2588, 2647, 2656, 2683, 2731, 2755, 2829, 2851, 2875, 2884, 2892, 2949, 2953},
    {49, 1133, 1162, 1199, 1273, 1281, 1327, 1338, 1412, 1445, 1467, 1476, 1542, 1563, 1588, 1636, 1659, 1687, 1729, 1758, 1794, 1824, 1957, 1983, 2134, 2168, 2176, 2181, 2197, 2242, 2265, 2288, 2323, 2444, 2456, 2462, 2485, 2521, 2572, 2628, 2651, 2718, 2743, 2771, 2789, 2829, 2847, 2882, 2911, 2973}};

int number[1459] = {
    0, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1131, 1132, 1133, 1134, 1135, 1136, 1137, 1138, 1139, 1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1151, 1152, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 1188, 1189, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1248, 1249, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269, 1271, 1272, 1273, 1274, 1275, 1276, 1277, 1278, 1279, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1321, 1322, 1323, 1324, 1325, 1326, 1327, 1328, 1329, 1331, 1332, 1333, 1334, 1335, 1336, 1337, 1338, 1339, 1341, 1342, 1343, 1344, 1345, 1346, 1347, 1348, 1349, 1351, 1352, 1353, 1354, 1355, 1356, 1357, 1358, 1359, 1361, 1362, 1363, 1364, 1365, 1366, 1367, 1368, 1369, 1371, 1372, 1373, 1374, 1375, 1376, 1377, 1378, 1379, 1381, 1382, 1383, 1384, 1385, 1386, 1387, 1388, 1389, 1391, 1392, 1393, 1394, 1395, 1396, 1397, 1398, 1399, 1411, 1412, 1413, 1414, 1415, 1416, 1417, 1418, 1419, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1428, 1429, 1431, 1432, 1433, 1434, 1435, 1436, 1437, 1438, 1439, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1451, 1452, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1461, 1462, 1463, 1464, 1465, 1466, 1467, 1468, 1469, 1471, 1472, 1473, 1474, 1475, 1476, 1477, 1478, 1479, 1481, 1482, 1483, 1484, 1485, 1486, 1487, 1488, 1489, 1491, 1492, 1493, 1494, 1495, 1496, 1497, 1498, 1499, 1511, 1512, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 1521, 1522, 1523, 1524, 1525, 1526, 1527, 1528, 1529, 1531, 1532, 1533, 1534, 1535, 1536, 1537, 1538, 1539, 1541, 1542, 1543, 1544, 1545, 1546, 1547, 1548, 1549, 1551, 1552, 1553, 1554, 1555, 1556, 1557, 1558, 1559, 1561, 1562, 1563, 1564, 1565, 1566, 1567, 1568, 1569, 1571, 1572, 1573, 1574, 1575, 1576, 1577, 1578, 1579, 1581, 1582, 1583, 1584, 1585, 1586, 1587, 1588, 1589, 1591, 1592, 1593, 1594, 1595, 1596, 1597, 1598, 1599, 1611, 1612, 1613, 1614, 1615, 1616, 1617, 1618, 1619, 1621, 1622, 1623, 1624, 1625, 1626, 1627, 1628, 1629, 1631, 1632, 1633, 1634, 1635, 1636, 1637, 1638, 1639, 1641, 1642, 1643, 1644, 1645, 1646, 1647, 1648, 1649, 1651, 1652, 1653, 1654, 1655, 1656, 1657, 1658, 1659, 1661, 1662, 1663, 1664, 1665, 1666, 1667, 1668, 1669, 1671, 1672, 1673, 1674, 1675, 1676, 1677, 1678, 1679, 1681, 1682, 1683, 1684, 1685, 1686, 1687, 1688, 1689, 1691, 1692, 1693, 1694, 1695, 1696, 1697, 1698, 1699, 1711, 1712, 1713, 1714, 1715, 1716, 1717, 1718, 1719, 1721, 1722, 1723, 1724, 1725, 1726, 1727, 1728, 1729, 1731, 1732, 1733, 1734, 1735, 1736, 1737, 1738, 1739, 1741, 1742, 1743, 1744, 1745, 1746, 1747, 1748, 1749, 1751, 1752, 1753, 1754, 1755, 1756, 1757, 1758, 1759, 1761, 1762, 1763, 1764, 1765, 1766, 1767, 1768, 1769, 1771, 1772, 1773, 1774, 1775, 1776, 1777, 1778, 1779, 1781, 1782, 1783, 1784, 1785, 1786, 1787, 1788, 1789, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 1799, 1811, 1812, 1813, 1814, 1815, 1816, 1817, 1818, 1819, 1821, 1822, 1823, 1824, 1825, 1826, 1827, 1828, 1829, 1831, 1832, 1833, 1834, 1835, 1836, 1837, 1838, 1839, 1841, 1842, 1843, 1844, 1845, 1846, 1847, 1848, 1849, 1851, 1852, 1853, 1854, 1855, 1856, 1857, 1858, 1859, 1861, 1862, 1863, 1864, 1865, 1866, 1867, 1868, 1869, 1871, 1872, 1873, 1874, 1875, 1876, 1877, 1878, 1879, 1881, 1882, 1883, 1884, 1885, 1886, 1887, 1888, 1889, 1891, 1892, 1893, 1894, 1895, 1896, 1897, 1898, 1899, 1911, 1912, 1913, 1914, 1915, 1916, 1917, 1918, 1919, 1921, 1922, 1923, 1924, 1925, 1926, 1927, 1928, 1929, 1931, 1932, 1933, 1934, 1935, 1936, 1937, 1938, 1939, 1941, 1942, 1943, 1944, 1945, 1946, 1947, 1948, 1949, 1951, 1952, 1953, 1954, 1955, 1956, 1957, 1958, 1959, 1961, 1962, 1963, 1964, 1965, 1966, 1967, 1968, 1969, 1971, 1972, 1973, 1974, 1975, 1976, 1977, 1978, 1979, 1981, 1982, 1983, 1984, 1985, 1986, 1987, 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2111, 2112, 2113, 2114, 2115, 2116, 2117, 2118, 2119, 2121, 2122, 2123, 2124, 2125, 2126, 2127, 2128, 2129, 2131, 2132, 2133, 2134, 2135, 2136, 2137, 2138, 2139, 2141, 2142, 2143, 2144, 2145, 2146, 2147, 2148, 2149, 2151, 2152, 2153, 2154, 2155, 2156, 2157, 2158, 2159, 2161, 2162, 2163, 2164, 2165, 2166, 2167, 2168, 2169, 2171, 2172, 2173, 2174, 2175, 2176, 2177, 2178, 2179, 2181, 2182, 2183, 2184, 2185, 2186, 2187, 2188, 2189, 2191, 2192, 2193, 2194, 2195, 2196, 2197, 2198, 2199, 2211, 2212, 2213, 2214, 2215, 2216, 2217, 2218, 2219, 2221, 2222, 2223, 2224, 2225, 2226, 2227, 2228, 2229, 2231, 2232, 2233, 2234, 2235, 2236, 2237, 2238, 2239, 2241, 2242, 2243, 2244, 2245, 2246, 2247, 2248, 2249, 2251, 2252, 2253, 2254, 2255, 2256, 2257, 2258, 2259, 2261, 2262, 2263, 2264, 2265, 2266, 2267, 2268, 2269, 2271, 2272, 2273, 2274, 2275, 2276, 2277, 2278, 2279, 2281, 2282, 2283, 2284, 2285, 2286, 2287, 2288, 2289, 2291, 2292, 2293, 2294, 2295, 2296, 2297, 2298, 2299, 2311, 2312, 2313, 2314, 2315, 2316, 2317, 2318, 2319, 2321, 2322, 2323, 2324, 2325, 2326, 2327, 2328, 2329, 2331, 2332, 2333, 2334, 2335, 2336, 2337, 2338, 2339, 2341, 2342, 2343, 2344, 2345, 2346, 2347, 2348, 2349, 2351, 2352, 2353, 2354, 2355, 2356, 2357, 2358, 2359, 2361, 2362, 2363, 2364, 2365, 2366, 2367, 2368, 2369, 2371, 2372, 2373, 2374, 2375, 2376, 2377, 2378, 2379, 2381, 2382, 2383, 2384, 2385, 2386, 2387, 2388, 2389, 2391, 2392, 2393, 2394, 2395, 2396, 2397, 2398, 2399, 2411, 2412, 2413, 2414, 2415, 2416, 2417, 2418, 2419, 2421, 2422, 2423, 2424, 2425, 2426, 2427, 2428, 2429, 2431, 2432, 2433, 2434, 2435, 2436, 2437, 2438, 2439, 2441, 2442, 2443, 2444, 2445, 2446, 2447, 2448, 2449, 2451, 2452, 2453, 2454, 2455, 2456, 2457, 2458, 2459, 2461, 2462, 2463, 2464, 2465, 2466, 2467, 2468, 2469, 2471, 2472, 2473, 2474, 2475, 2476, 2477, 2478, 2479, 2481, 2482, 2483, 2484, 2485, 2486, 2487, 2488, 2489, 2491, 2492, 2493, 2494, 2495, 2496, 2497, 2498, 2499, 2511, 2512, 2513, 2514, 2515, 2516, 2517, 2518, 2519, 2521, 2522, 2523, 2524, 2525, 2526, 2527, 2528, 2529, 2531, 2532, 2533, 2534, 2535, 2536, 2537, 2538, 2539, 2541, 2542, 2543, 2544, 2545, 2546, 2547, 2548, 2549, 2551, 2552, 2553, 2554, 2555, 2556, 2557, 2558, 2559, 2561, 2562, 2563, 2564, 2565, 2566, 2567, 2568, 2569, 2571, 2572, 2573, 2574, 2575, 2576, 2577, 2578, 2579, 2581, 2582, 2583, 2584, 2585, 2586, 2587, 2588, 2589, 2591, 2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599, 2611, 2612, 2613, 2614, 2615, 2616, 2617, 2618, 2619, 2621, 2622, 2623, 2624, 2625, 2626, 2627, 2628, 2629, 2631, 2632, 2633, 2634, 2635, 2636, 2637, 2638, 2639, 2641, 2642, 2643, 2644, 2645, 2646, 2647, 2648, 2649, 2651, 2652, 2653, 2654, 2655, 2656, 2657, 2658, 2659, 2661, 2662, 2663, 2664, 2665, 2666, 2667, 2668, 2669, 2671, 2672, 2673, 2674, 2675, 2676, 2677, 2678, 2679, 2681, 2682, 2683, 2684, 2685, 2686, 2687, 2688, 2689, 2691, 2692, 2693, 2694, 2695, 2696, 2697, 2698, 2699, 2711, 2712, 2713, 2714, 2715, 2716, 2717, 2718, 2719, 2721, 2722, 2723, 2724, 2725, 2726, 2727, 2728, 2729, 2731, 2732, 2733, 2734, 2735, 2736, 2737, 2738, 2739, 2741, 2742, 2743, 2744, 2745, 2746, 2747, 2748, 2749, 2751, 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759, 2761, 2762, 2763, 2764, 2765, 2766, 2767, 2768, 2769, 2771, 2772, 2773, 2774, 2775, 2776, 2777, 2778, 2779, 2781, 2782, 2783, 2784, 2785, 2786, 2787, 2788, 2789, 2791, 2792, 2793, 2794, 2795, 2796, 2797, 2798, 2799, 2811, 2812, 2813, 2814, 2815, 2816, 2817, 2818, 2819, 2821, 2822, 2823, 2824, 2825, 2826, 2827, 2828, 2829, 2831, 2832, 2833, 2834, 2835, 2836, 2837, 2838, 2839, 2841, 2842, 2843, 2844, 2845, 2846, 2847, 2848, 2849, 2851, 2852, 2853, 2854, 2855, 2856, 2857, 2858, 2859, 2861, 2862, 2863, 2864, 2865, 2866, 2867, 2868, 2869, 2871, 2872, 2873, 2874, 2875, 2876, 2877, 2878, 2879, 2881, 2882, 2883, 2884, 2885, 2886, 2887, 2888, 2889, 2891, 2892, 2893, 2894, 2895, 2896, 2897, 2898, 2899, 2911, 2912, 2913, 2914, 2915, 2916, 2917, 2918, 2919, 2921, 2922, 2923, 2924, 2925, 2926, 2927, 2928, 2929, 2931, 2932, 2933, 2934, 2935, 2936, 2937, 2938, 2939, 2941, 2942, 2943, 2944, 2945, 2946, 2947, 2948, 2949, 2951, 2952, 2953, 2954, 2955, 2956, 2957, 2958, 2959, 2961, 2962, 2963, 2964, 2965, 2966, 2967, 2968, 2969, 2971, 2972, 2973, 2974, 2975, 2976, 2977, 2978, 2979, 2981, 2982, 2983, 2984, 2985, 2986, 2987, 2988, 2989, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998, 2999};
// typedef struct
// {
//     KeyType key;
//     int value;
// } VertexType; //顶点类型定义

// typedef struct ArcNode
// {                            //表结点类型定义
//     int adjvex;              //顶点位置编号
//     struct ArcNode *nextarc; //下一个表结点指针
// } ArcNode;
// typedef struct VNode
// {                      //头结点及其数组类型定义
//     VertexType data;   //顶点信息
//     ArcNode *firstarc; //指向第一条弧
// } VNode, AdjList[MAX_VERTEX_NUM];
// struct IGraph
// {
//     AdjList vertices;
//     int vexnum, arcnum; //顶点数、弧数
// };