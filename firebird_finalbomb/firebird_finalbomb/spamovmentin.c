int l3=0,c[49];
void store(int y)
{

	c[l3]=y;
	l3++;

}

void spamovement()
{ //north=0;east=1;west=3;south=2
	int robo1=3;
	int direction=3;
	int r,f,i=0,d;
	
	int a1=robo1/7;
	int b=0;
	for(r=0;r<49;r++)
	{
		c[r]=99;
	}
store(robo1);



	while(1)
	{
		if(a1==0 && b==0 )
		{
			while(robo1!=6)
			{
				robo1++;

				store(robo1);
			}
		}
		else if(a1==6 && b==6)
		{
			while(robo1!=42)
			{
				robo1--;

				store(robo1);
			}
		}
		if(a1==b )
		{

			break;

		}

		if(direction==3 || r==2 )
		{f=robo1/7;
			if(r==2)
			{
				direction=3;
				robo1--;
				r=0;
				store(robo1);

			}

			if((robo1%7)==0)
			{
				if(((b+1)!=a1) || ((a1+1)!=b ))
				{
					if(f==a1)
					{direction=99;
						i=robo1;
						robo1=b*7;
						while(i != robo1 && i-7!=robo1)
						{ i-=7;


							store(i);

						}
						r=1;

						a1--;

						store(robo1);

					}
					else
					{
						direction=99;
						i=robo1;
						robo1=a1*7;

						while(i != robo1 && i+7!=robo1)
						{ i+=7;


							store(i);

						}

						r=1;
						b++;
						store(robo1);

					}
				}
			}
			else
			{
				robo1--;
				store(robo1);

			}

		}




		else if(direction==1 || r==1)
		{f=robo1/7;
			if(r==1)
			{
				direction=1;
				robo1++;
				store(robo1);
				r=0;

			}

			if((robo1-6)%7==0)
			{
				if(((b+1)!=a1 || (a1+1)!=b ))
				{
					if(f==b)
					{
						direction=99;
						i=robo1;
						robo1=a1*7+6;r=2;
						while(i!=robo1 && i+7!=robo1)
						{
							i+=7;

							store(i);

						}
						b++;
						store(robo1);


					}

					else
					{direction =99;
						i=robo1;
						robo1=b*7+6;r=2;
						while(i!=robo1 && i-7!=robo1)
						{
							i-=7;
							store(i);

						}
						a1--;
						store(robo1);



					}
				}
			}
			else
			{
				robo1++;
				store(robo1);



			}


		}

	}

}
