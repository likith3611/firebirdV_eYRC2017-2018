
int px,qx,z;
const int pb=0;
 int c[49];
 int d10;
 int map[49];
 int path_finder(int u9,int st,int r40[3] )
{

    int x,g=0;
     int num=st;


for(x=0;x<49;x++)
{
	map[x]=99;
}	
    int des= u9;
	for(x=0;x<3;x++)
	{
		for(d10=0;d10<49;d10++)
		{
			if(r40[x]==d10)
			{
				map[d10]=100;
			}
		}
	}
	
    d10=0;
	c[d10]=num;
	d10++;
   while(1)
   {
comapartor(num,des);


if(g==1)
{
   if(pb!=z)
   {

         if(px<qx)
                ++num;
                else if(px>qx)
                --num;
                if((num-7)==des)
                {
                    g=0;
                }
                else if((num+7)==des)
                {
                    g=0;
                }
            c[d10]=num;
            d10++;

   }
}
    if(num==des)
    {break;
    }

     if(((des-num) > 0 )&& (z!=0) )
       {
            x= obstacle_checker(num,1,map);

            if(x==2)
            {
             ++num;

            }
            else if(x==8)
            {
         --num;

            }
            else if(x==10)
            {
                if(px<qx)
                ++num;
                else if(px>qx)
                --num;
                else
                --num;


            }
            else
            {
                num+=7;
            }
            c[d10]=num;
            d10++;
            
       }
       else if(((des - num)<0) && (z !=0) )
       {
           x=obstacle_checker(num,4,map);
             if(x==2)
            {
               ++num;
            }
            else if(x==8)
            {
            --num;
            }
            else if(x==10)
            {

                if(px<qx)
                ++num;
                else if(px>qx)
                --num;
                else
                --num;

            }
            else
            {
               num-=7;
            }

           c[d10]=num;
           d10++;
          
       }
       else
       {

g=1;

                 x=obstacle_checker(num,2,map);
                   if(x==15)
            {

                if(px<qx)
                ++num;
                else if(px>qx)
                --num;
                else
                --num;
            }
           else if((x==11) || (x==14))
            {
                   if(px<qx)
                ++num;
                else if(px>qx)
                --num;
            }
            else if(x==13)
            {
                if(px<qx)
                num-=7;
                else if(px>qx)
                --num;

            }
            else if(x==7)
            {
               if(px<qx)
                num++;
                else if(px>qx)
                num-=7;
            }
            else if(x==1||x==5)
            {
              num-=7;
            }
            else if(x==12||x==6)
            {if(qx>px)
            num+=1;
            else if(qx<px)
                num-=1;
            else
                num+=7;
            }
            else if(x==9||x==3)
            {if(px<qx)
            num+=1;
            else if(px>qx)
                num-=1;
            else
                num-=7;

            }

            else
            {
                 num+=7;
            }





            c[d10]=num;
            d10++;
           

       }
	   


   }
   for(num=0;num<49;num++)
   {
	   map[num]=99;
   }


return d10;
 
}

int obstacle_checker(int pack,int r,int map[49])
{
    int i1=0;
    if ( map[pack+7]==99  && (r ==1 || r==2) &&(pack < 42) && (pack !=42 || pack != 48) )
    {
        i1=i1+4;
    }
    if(map[pack-1]==99 && ((pack%7) != 0) )
    {
        i1=i1+8;
    }
if(map[pack+1]==99 && (((pack-6)%7) != 0))
{
    i1=i1+2;
}
if(map[pack-7]==99 && (r ==4 || r==2) && (pack >= 7) && (pack !=0 || pack!=6))
{
    i1=i1+1;
}

    return i1;
}
void comapartor(int num,int des)
{
    int l1=num/7;
    int m=num%7;

    px=l1;
    qx=m;
   
    l1=des/7;
    m=des%7;
    z=px-l1;
   
    px=(qx+1)-m;
    qx=(qx-1)-m;
    px=abs(px);
    qx=abs(qx);

}
