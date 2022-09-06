/*
 * @Description: 数独读取及求解模块
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-09-03 09:44:50
 * @LastEditTime: 2022-09-06 09:52:22
 */
// status drawInitialPanel()
// {
//     for (int i = 0; i < 15; i++)
//     {
//         if (i >= 0 && i <= 5)
//         {
//         }
//     }
// }

// status printPanel(int data[2][9][9])
// {
//     //打印上半部分
//     for (int y = 0; y <= 5; y++)
//     {
//         for (int x = 0; x <= 8; x++)
//         {
//             printf("%d ", data[1][x][y]);
//             if ((x + 1) % 3 == 0)
//                 printf(" ");
//         }
//         printf("\n");
//         if ((y + 1) % 3 == 0)
//             printf("\n");
//     }
//     //打印中间重叠部分
//     for (int y = 6; y <= 8; y++)
//     {
//         for (int x = 0; x <= 14; x++)
//         {
//             if (x >= 0 && x <= 8)
//             {
//                 printf("%d ", data[1][x][y]);
//                 if ((x + 1) % 3 == 0)
//                     printf(" ");
//             }
//             else
//             {
//                 printf("%d ", data[2][x - 6][y - 6]);
//                 if ((x + 1) % 3 == 0)
//                     printf(" ");
//             }
//         }
//         printf("\n");
//         if ((y + 1) % 3 == 0)
//             printf("\n");
//     }
//     //打印下半部分
//     for (int y = 3; y <= 8; y++)
//     {
//         printf("                     ");
//         for (int x = 0; x <= 8; x++)
//         {
//             printf("%d ", data[2][x][y]);
//             if ((x + 1) % 3 == 0)
//                 printf(" ");
//         }
//         printf("\n");
//         if ((y + 1) % 3 == 0)
//             printf("\n");
//     }
// }
