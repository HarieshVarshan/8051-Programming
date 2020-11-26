#include<reg51.h>
void delay()
{
	int i,j;
	for(i=0;i<50;i++)
	 for(j=0;j<1275;j++);
}
void disp(int x,int count)
{
	int i,j,y;
	char a[]={0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X67};
	if(count%2==0)
		y=1;
	else
		y=0;
	for(i=x;i>=y;i--)
	{
		for(j=9;j>=0;j--)
		{
			P2=a[i];
			P3=a[j];
			delay();
		}
	}
	count++;
}
void main()
{
	int i,j,count=0;
	char b[]={0X44,0X11,0X44,0X22,0X11,0X44,0X22,0X44,0X88,0X11,0X88,0X22,0X11,0X88,0X22,0X88};
	P0=0X00;
	P1=0X00;
	P2=0X00;
	P3=0X00;
	j=0;
	while(1)
	{
	for(i=0;i<16;i++)
	{
		P0=b[i];
		P1=b[i+1];
		if(j%2==0)
		{
			disp(4,count);
		}
		else
		{
			disp(0,count);
		}
		j++;
		i++;
		count++;
	}
}}
	