# Energy Not Included - Player README

# 1. Overview

>**Energy Not Included** เป็นเกมแนว **Turn-based Resource Management** สำหรับผู้เล่น **1-3 คน** ผู้เล่นจะรับบทเป็นเจ้าของบริษัทพลังงาน แข่งขันกันสร้างระบบผลิตไฟฟ้า ตั้งแต่การหาแหล่งทรัพยากร สร้างโรงงานไฟฟ้า เชื่อมต่อเส้นทางส่งพลังงาน และขายไฟฟ้าให้เมืองต่าง ๆ
>
>เกมรันบน **Web Browser** โดยใช้ **HTML / CSS / JavaScript** เป็นส่วนแสดงผล และใช้ **C++** เป็นระบบเบื้องหลังของเกม

## Objective

เป้าหมายของเกมคือ **บริหารพลังงานให้ดีที่สุดและทำคะแนนรวมให้สูงที่สุด**

เกมจะจบเมื่อ **เมืองทุกเมืองในแผนที่ได้รับไฟฟ้าครบตามเงื่อนไข** หลังจากนั้นระบบจะคำนวณคะแนนรวมของผู้เล่นแต่ละคน ผู้เล่นที่มีคะแนนสูงที่สุดจะเป็นผู้ชนะ



## Basic Rules

- ผู้เล่นเริ่มเกมด้วยเงินเริ่มต้น **300 coins**
- ผู้เล่นแต่ละคนมี **3 Action Points ต่อเทิร์น**
- 1 การกระทำ ใช้ 1 Action Point และอาจใช้ coins ตามราคาของสิ่งที่สร้าง
- ผู้เล่นต้องสร้างสิ่งก่อสร้างให้ตรงกับประเภทของ Node
- Resource Plant ต้องสร้างบน Resource Node
- Power Plant ต้องสร้างบน Power Plant Node
- Link ใช้เชื่อมต่อ Node เพื่อส่งทรัพยากรหรือพลังงาน
- ถ้าผู้เล่นไม่มี Action Point เหลือ เทิร์นจะเปลี่ยนไปยังผู้เล่นคนถัดไป


## Node Types

| Node | หน้าที่ |
|---|---|
| Resource Node | จุดสำหรับสร้าง Resource Plant เพื่อผลิตทรัพยากร |
| Power Plant Node | จุดสำหรับสร้าง Power Plant เพื่อผลิตไฟฟ้า |
| Small City Node | เมืองขนาดเล็กที่รับไฟฟ้าและให้คะแนน |
| Big City Node | เมืองขนาดใหญ่ที่ต้องการไฟมากกว่า แต่ให้คะแนนสูงกว่า |


# 2. How to Play

## วิธีการเปิดเกม

1. run sever.exe ใน floder backend
2. run index.html

## ลำดับการเล่น

1. เปิดเกมแล้วเลือกจำนวนผู้เล่น
2. กดเริ่มเกมเพื่อเข้าสู่หน้าแผนที่
3. ในเทิร์นของตนเอง ให้เลือก Action ที่ต้องการทำ
4. สร้าง Resource Plant เพื่อเริ่มผลิตทรัพยากร
5. สร้าง Power Plant เพื่อแปลงทรัพยากรหรือพลังงานธรรมชาติเป็นไฟฟ้า
6. สร้าง Link เพื่อส่งทรัพยากรไปยังโรงไฟฟ้า และส่งไฟฟ้าไปยังเมือง
7. เมื่อใช้ครบ 3 Action Points หรือไม่ต้องการทำต่อ ให้จบเทิร์น
8. เล่นวนไปจนกว่าเมืองทุกเมืองจะได้รับไฟฟ้า
9. ระบบคำนวณคะแนนรวม และประกาศผู้ชนะ

ป.ล. ข้อที่ 4-6 เป็นแค่ตัวอย่างการออก action ผู้เล่นสามารถออกแบบลำดับ action ของตนเองได้อย่างอิสระ

>## Main Actions
>
>### 1. Create Resource Plant
>
>ใช้สร้างโรงงานผลิตทรัพยากรบน **Resource Node**
>
>ทรัพยากรในเกมมี 4 ประเภท:
>
>- Coal / ถ่านหิน
>- Gas / ก๊าซธรรมชาติ
>- Biomass / ชีวมวล
>- Uranium / ยูเรเนียม
>
>Resource Plant จะผลิตทรัพยากรเพื่อนำไปใช้กับ Power Plant บางประเภท
>
>
>### 2. Create Power Plant
>
>ใช้สร้างโรงงานไฟฟ้าบน **Power Plant Node**
>
>ตัวอย่างประเภทโรงไฟฟ้า:
>
>- Coal Plant
>- Gas Plant
>- Biomass Plant
>- Nuclear Plant
>- Solar Plant
>- Wind Plant
>- Hydro Plant
>
>Power Plant บางชนิดต้องใช้ทรัพยากรจาก Resource Plant ส่วนบางชนิดใช้ค่าจากสภาพแวดล้อม เช่น แสงแดด ลม หรือน้ำ
>
>
>### 3. Create Link
>
>ใช้เชื่อม Node 2 จุดเข้าด้วยกัน เพื่อส่งของระหว่าง Node
>
>การเชื่อมต่อหลักมี 2 แบบ:
>
>| การเชื่อมต่อ | ผลลัพธ์ |
>|---|---|
>| Resource Node → Power Plant Node | ส่งทรัพยากรไปผลิตไฟฟ้า |
>| Power Plant Node → City Node | ส่งไฟฟ้าเข้าเมือง |


## Scoring

| ประเภทคะแนน | วิธีคิดคะแนน |
|---|---|
| Coins | 1 coin = 1 คะแนน |
| Power Plant | ได้คะแนนจาก 75% ของราคาสร้างโรงไฟฟ้า |
| Resource Plant | ได้คะแนนจาก 70% ของราคาสร้างโรงงานทรัพยากร |
| Small City | เมืองละ 75 คะแนน |
| Big City | เมืองละ 150 คะแนน |
| Waste | 1 waste = -2 คะแนน |

คะแนนรวมคำนวณจาก:

**Coins + Building Value + City Points - Waste**


## Player Tips

- อย่าสร้างโรงไฟฟ้าโดยไม่มีแหล่งทรัพยากรรองรับ
- เมืองใหญ่ให้คะแนนสูง แต่ต้องใช้พลังงานมากกว่า
- ควรเชื่อม Resource Node ไปยัง Power Plant ก่อนเชื่อม Power Plant ไปยังเมือง
- ระวัง Waste เพราะจะถูกหักคะแนนตอนจบเกม โดย waste เกิดขึ้นได้จาก Power Plant และ Resource Plant ซึ่งจะเพิ่มขึ้นทุกๆครั้งที่ถึงเทิร์นของตนเอง


## Controls

| Control | การใช้งาน |
|---|---|
| Left Click | เลือกหรือวางสิ่งก่อสร้างบนแผนที่ |
| Tool / Action Button | เลือก Action ที่ต้องการทำ |
| End Turn | จบเทิร์นและเปลี่ยนไปผู้เล่นถัดไป |
| Player Count Button | เลือกจำนวนผู้เล่นก่อนเริ่มเกม |


## End Game

เกมจะจบเมื่อ **ทุกเมืองได้รับไฟฟ้าแล้ว** จากนั้นระบบจะรวมคะแนนของผู้เล่นทุกคน ผู้เล่นที่มีคะแนนรวมสูงที่สุดจะเป็นผู้ชนะของเกม

# 3. Data เพิ่มเติมสำหรับการคำนวณคะแนน และ waste

## Power Plant

| Power Plant Data | EFF (%) | Build Cost (Coin k) | Resource (Type) | Resource (INPUT kg) | Max OUTPUT | Waste (%) |
| :--- | :---: | :---: | :--- | :---: | :---: | :---: |
| Coal Plant | 0.8 | 80 | Coal | 10 | 8 | 0.6 |
| Gas Plant | 1.15 | 140 | Gas | 8 | 9.2 | 0.5 |
| BiomassPlant | 0.75 | 120 | Biomass | 6 | 4.5 | 0.4 |
| Solar Plant | 1 | 160 | Solar | 8 | 8 | 0 |
| Wind Plant | 1.05 | 180 | Wind | 9 | 9.45 | 0 |
| Hydro Plant | 0.35 | 250 | Water | 15 | 5.25 | 0.1 |
| Nuclear Plant | 2.5 | 1200 | Uranium | 2 | 5 | 4 |

waste = input * waste(%)

## Resource Plant

| Resource Plant Data | Build Cost (Coin k) | Product (kg) | Waste |
| :--- | :---: | :---: | :---: |
| Coal Plant | 100 | 10 | 8 |
| Gas Plant | 160 | 8 | 4.8 |
| BiomassPlant | 180 | 6 | 3 |
| Nuclear Plant | 500 | 2 | 1.8 |


---
# ขอให้สนุก 😉



CLIP แก้ไข ที่อัดหน้าจอไม่ติดค่ะ
https://youtu.be/9DQ3uGvfxt4