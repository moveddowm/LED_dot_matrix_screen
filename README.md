## 0、准备
> 操作就是跟着[https://www.bilibili.com/video/BV1Mb411e7re/](https://www.bilibili.com/video/BV1Mb411e7re/)的资源做的，算是总结，自用回顾

使用51单片机 ，采用74HC595的串行输入并行输出的移位寄存器,LED点阵屏。
特点：没有，就是学会使用入门，后续我会尝试移植到stm32，和用其他的方式实现。
## 1、理论知识
![](https://cdn.nlark.com/yuque/0/2023/png/12377426/1684402035341-01a3a4fb-4fe0-45a5-985a-0bf6f9735372.png#averageHue=%23efedec&from=url&id=GyMSb&originHeight=543&originWidth=1060&originalType=binary&ratio=1.6500000953674316&rotation=0&showTitle=false&status=done&style=none&title=)
SER写入数据内容   SERCLK 上升沿写入    移位寄存器，高位先写入  RCLK为高电平就写入
![](https://cdn.nlark.com/yuque/0/2023/png/12377426/1684405402798-0bc6f648-3802-467a-b0e1-b9b11a874636.png?x-oss-process=image%2Fresize%2Cw_658%2Climit_0#averageHue=%23e2dedd&from=url&id=BrnBz&originHeight=516&originWidth=658&originalType=binary&ratio=1.6500000953674316&rotation=0&showTitle=false&status=done&style=none&title=)
跟控制数码管一样，先一列低电平，然后再对应的led行给高电平就可以实现控制
## 2、代码实现过程
> 这里说一下，对应的IO口和简单函数功能我都没有写，因为其一这是一个复习，其二不同对应的IO口也不一样目前我认为没有必要再表述一遍IO口对应

1. 74HC595的写入数据
```c
void _74HC95_WR(unsigend char byte)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        SER=byte&(0x80>>i);//去高位然后循环移位操作
        SCK=1;  //高电平写
        SCK=0; 
    }
    RCk=1;//高电平发送
    RCK=0;
}
```

2. LED点阵屏显示数据
```c
void  LED_dot_matrix_screen(unsigned char List,data)
{
    _74HC95_WR(data);
     LED_dot_IO=~(0x80>>List);
    Delay(1);
    LED_dot_IO=0xFF; //消隐操作
}
```

3. 参数的初始化
```c
void LED_dot_init()
{
    SCK=0;
    RCK=0;
}
```
4.动画数据
这个动画数据的意思，是在LED点阵屏中显示的每一列的的数据（就是一列亮的灯），动画的就是以多少时间间隔后出现一个新的画面，所以现在显示一个画面，间隔假设15ms，在出现一个画面。
![image.png](https://cdn.nlark.com/yuque/0/2023/png/12377426/1684408699897-0131b3f0-90da-492f-9209-ebb9fcd6258d.png#averageHue=%232a932a&clientId=u612101cd-9064-4&from=paste&height=264&id=ufb30cd48&originHeight=825&originWidth=1163&originalType=binary&ratio=1.6500000953674316&rotation=0&showTitle=false&size=157916&status=done&style=none&taskId=u0f8a5de8-fd55-44cd-8da6-73e48684744&title=&width=372.84844970703125)使用这个软件`子模提取`,设置第一个画面
得到第一个画面![image.png](https://cdn.nlark.com/yuque/0/2023/png/12377426/1684408814207-dd769d5a-dcfe-4ca3-8044-b0430ab7fa7d.png#averageHue=%232a9329&clientId=u612101cd-9064-4&from=paste&height=393&id=u5c324b1a&originHeight=649&originWidth=747&originalType=binary&ratio=1.6500000953674316&rotation=0&showTitle=false&size=104979&status=done&style=none&taskId=uaced2120-e32a-410b-b939-52cbd1375e4&title=&width=452.72724656034256)得到第二个画面![image.png](https://cdn.nlark.com/yuque/0/2023/png/12377426/1684408844121-48130acb-4b4c-40be-8eb8-57e9756e0975.png#averageHue=%232a9329&clientId=u612101cd-9064-4&from=paste&height=356&id=u8b46b83c&originHeight=588&originWidth=794&originalType=binary&ratio=1.6500000953674316&rotation=0&showTitle=false&size=95445&status=done&style=none&taskId=udb3b821d-d9de-4a81-95c2-7ebebdbe300&title=&width=481.21209339881125)
```c
unsigned char code Animation[]=
{
    0x00,0x01,0x0A,0x1C,0x0B,0x00,0x00,0x00,

    0x00,0x00,0x0B,0x1C,0x0A,0x01,0x00,0x00,
}
```

5. 实现画面切换的代码实现
```c
unsigned char count,offest=0; //count 用来计数，实现多少ms跳到下一个画面  offest 偏移实现切换画面

while(1)
{
    for(int i=0,i<8;i++)
    {
        Animation[i+offest];//显示8个列，偏移
    }count ++;
    if(count>15)
    {
        count=0;
        offest+=8;
        if(offest>strlen(Animation)-8)offset=0;
    }
}
```
## 3、源码
