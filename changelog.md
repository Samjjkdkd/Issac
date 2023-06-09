## **ISAAC v2.1**

- ### 两个敌人
- ### 英雄血量

## **ISAAC v2.2**

- ### 新加血量条。

## **ISAAC v2.3**

- ### 新增敌人
- ### 开始界面

## **ISAAC v2.4**

- ### 新增飙血特效
- ### 飙血特效2还没有实装，在res里，有心人可以实装一下（按概率随机出现特效1和2）

## **ISAAC v2.5**

- ### 解决了hero的旋转问题

## **ISAAC v2.6**

- ### 血迹增加

## **ISAAC v2.6.1**

- 解决了血量加载时的问题，现在可以设置任意多的血量数组大小而不影响加载时间

## **ISAAC v2.7**

- ### 新增战技

## **ISAAC v2.7.1**

- ### 删除了爆炸音效，解决了卡顿的问题

## **ISAAC v2.7.2**

- ### 新增战技显示

    包括战技按键，冷却时间显示

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/1.png)

## **ISAAC v2.7.3**

- ### 新增爆发显示

    包括元素爆发按键，充能指示（并没有实装元素爆发技能）

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/2.png)

## **ISAAC v2.8**

- ### 新增能量球

    击败敌人（非创亖敌人）后有概率产生能量球。元素爆发的能量积攒方式也由释放战技改为拾取能量球。场上能量球存在上限，每个能量球产出持续一段时间后消失。能量球动画由两帧构成，以达到闪烁效果

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/3.png)

## **ISAAC v2.8.2**

- ### 完善爆发逻辑

    添加元素爆发cd显示，并使得元素爆发能够释放（并无实际功能，仅用于完善充能与释放逻辑）

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/4.png)

## **ISAAC v2.8.3**

- ### 修改技能图标透明度

## **ISAAC 2.9.0**

- ### 新增冲刺机制

    原本血条下方的蓝条被改为体力条（灰条），shift键功能修改为冲刺，冲刺会消耗体力（具体机制类似于Nesh'Naury）

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/5.png)

## **ISAAC v2.9.1**

- ### 说明文档格式改进&归一化

## **ISAAC v2.9.2**

- ### 修改按键逻辑

    部分按键（如E/Q/Shift）均要求抬起后才能再次触发，故在*MoveVector*中新增按键抬起的相应
    
## **ISAAC v2.10.0**

- ### 元素爆发功能实装

    元素爆发(Q)现已具有实际功能，目前为时停敌方所有行为持续一段时间，这段时间*Hero*可以自由移动和发射子弹  

    时停期间场景以及敌机会变暗 

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/6.png)

## **ISAAC v2.10.1**

- ### 元素爆发功能修改

    Q技能会让子弹发射间隔减半，这个代码写的有点傻逼，好心人可以优化一下（我改在hero::shoot()了）
    
## **ISAAC v3.0.0**

- ### 新增终结技

    按下Z释放终结技。释放元素爆发以给终结技充能。

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/7.png)

## **ISAAC v3.1**

- ### e修改为扇形。开q时候速度和射速增加

   好心人可以把开始界面写成纯代码形式这样就可以加音乐了。

## **ISAAC v3.1.1**

- ### 优化终结技动画

## **ISAAC v3.1.2**

- ### Bug fix

## **ISAAC v3.1.3**

- ### 实验性功能：修改移动方式为WASD，具体操作方式同开车类游戏

## **ISAAC v3.2.0**

- ### 正式添加上版本的移动方式，可在开始界面选择

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/8.png)

## **ISAAC v3.3.-1**

- ### 代码大重构（Work in progress）

    趁现在还没出bug赶紧push一下

- ### 加入FPS显示（这样才能知道优化做的有多烂）

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/9.png)

## **ISAAC v3.3.0**

- ### 代码大重构（Stage I clear）

    *Hero*模块已基本完成全部重构

- ### 加入作弊按键

    暂无更多说明

- ### 优化了上个版本的掉帧问题

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/10.png)

## **ISAAC v3.4.0**

- ### 代码大重构

    全部代码已基本完成重构，已做好对接Roguelike准备工作

    修改部分逻辑，性能优化

- ### 终结技动画优化

    终结技动画不再依赖*timer*计时，而是采用真实事件计时，使音画不同步问题彻底解决

    增加、完善动画细节，时间对齐

    播放该动画时不会显示游戏界面的各类提示信息

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/11.png)

## **ISAAC v3.5.0**

- ### 调整启动对话框选项文字

    其实只是排了下版

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/12.png)

- ### 新增场景切换功能

    *MainScene*可管理多个场景，并运用*connect*使不同之间进行切换
    
    目前只实装了一个欢迎界面

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/13.png)

## **ISAAC v4.0.0**

- ### 调整输入方式

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/14.png)
    
- ### 调整游戏相关配置

    修正冲刺时间

    修正敌机越来越少的问题

- ### 场景间的切换添加转场动画

    黑色淡出淡入遮罩

- ### 增加结算界面

    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/15.png)

- ### 更换相应场景背景音乐

    选用了两首CXXX自主创作的音乐：PANIC! 与 Shooting Stars

## **ISAAC v4.0.1**

- ### 增加程序鲁棒性

    主要用于应对开发者模式可能产生的潜在的bug

## **ISAAC v4.0.2**

- ### Bug fix

## **ISAAC v4.1.2**

- ### 加入了一个会冲刺的无敌敌怪
       
    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/16.png)


- ### 会在冲刺前显示轨迹
       
    ![image](https://raw.githubusercontent.com/Samjjkdkd/Issac/main/img/17.png)





