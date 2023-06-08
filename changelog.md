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