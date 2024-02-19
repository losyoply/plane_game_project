for (int i = 0; i<MAX_ENEMY; i++)
        {
            dirx = rand()%2+1;
            if (enemies[i].hidden == true)
                continue;
            if(dirx == 1 && enemies[i].x <= SCREEN_W- enemies[i].w/2) enemies[i].x += enemies[i].vx;
            else if(dirx ==2 && enemies[i].x >= enemies[i].w/2) enemies[i].x -= enemies[i].vx;
            enemies[i].y += sqrt((enemies[i].v*enemies[i].v) - (enemies[i].vx*enemies[i].vx));
            if(point>=10)
            {
                    enemies[i].y  += 2*point/10;
            }
            if (enemies[i].y >= SCREEN_H-(enemies[i].w/2))
                enemies[i].hidden = true;
        }
        //hide enemies remake
        for (int i = 0; i<MAX_ENEMY;i++)
        {
            if (enemies[i].hidden == true&&point<BOSSPOINT)
            {
                enemies[i].hidden = false;
                enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
                enemies[i].y = 0;
                enemies[i].v = (rand() % 5) + 1 + 8; //ŽŠ­‘½‰õ
                enemies[i].vx = rand() % (int)(enemies[i].v-8) +1 + 7;
            }
        }
