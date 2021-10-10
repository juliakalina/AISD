#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW
#include <vld.h>


#include "queue.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>


main()
{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		bool error = false;
		while (true && !error)
		{
			int slaves_num = GetInt("number of slaves");
			int angle_min = GetInt("min angle");
			int angle_max = GetInt("max angle");
			double eps = GetDouble("epsilon");
			int members_min = GetInt("min members");
			int members_max = GetInt("max members");

			int g;
			

			angle_min %= 360;
			angle_max %= 360;

			double* table = NULL;
			queue_t** queue_table = NULL;
			char* cur_step = NULL;

			int angle_delta = angle_max - angle_min + 1;
			table = (double*)calloc(angle_delta, sizeof(double));

			queue_table = (queue_t**)calloc(slaves_num, sizeof(queue_t*));

			for (int i = 0; i < slaves_num; ++i)
			{
				queue_table[i] = (queue_t*)calloc(2, sizeof(queue_t));

				AllocateQueue(&queue_table[i][0]);
				AllocateQueue(&queue_table[i][1]);
			}

			cur_step = (char*)calloc(slaves_num, sizeof(char));

			bool areQueuesEmpty = true;
			for (int angle = 0; angle != angle_delta || !areQueuesEmpty;)
			{
				areQueuesEmpty = true;
				for (int i = 0; i < slaves_num; ++i)
				{
					int probability = rand() % 4;
					if (probability < 3)
					{
						if (cur_step[i] == 0)//ýòàï âûïîëíåíèÿ
						{
							int members = rand() % (members_max - members_min) + members_min;

							if (angle != angle_delta)
							{
								task_t task = { angle_min + angle++, members, 0.0 };

								PushQueue(&queue_table[i][0], task);
							}

							cur_step[i]++;
						}
						else if (cur_step[i] == 1)
						{
							while (!IsEmptyQueue(queue_table[i][0]))
							{
								task_t task = PopQueue(&queue_table[i][0]);

								task.res = CalcSin(task.members_num, task.angle, eps);

								PushQueue(&queue_table[i][1], task);
							}

							cur_step[i]++;
						}
						else if (cur_step[i] == 2)
						{
							while (!IsEmptyQueue(queue_table[i][1]))
							{
								task_t task = PopQueue(&queue_table[i][1]);

								table[task.angle - angle_min] = task.res;
							}

							if (angle == angle_delta)
								cur_step[i] = 2;
							else
								cur_step[i] = 0;
						}
					}

					if (!IsEmptyQueue(queue_table[i][0]) || !IsEmptyQueue(queue_table[i][1]))
						areQueuesEmpty = false;
				}
			}

			for (int i = 0; i < angle_delta; ++i)
				printf("angle:%3u res:%lf\n", i + angle_min, round(table[i] / eps) * eps);

			printf("\n");
			free(cur_step);

			if (queue_table)
				for (int i = 0; i < slaves_num; ++i)
				{
					if (queue_table[i])
					{
						DeallocateQueue(&queue_table[i][0]);
						DeallocateQueue(&queue_table[i][1]);
					}

					free(queue_table[i]);
				}

			free(queue_table);
			free(table);
			printf("Do you want to leave the program?\n");
			scanf_s("%d", &g);
			if (!g)
				continue;
			else
				return 0;
}
			_CrtDumpMemoryLeaks();
			if (error)
				return -1;
			else return 0;
}
