# Energy Not Included — Player README

> เอกสารนี้ทำสำหรับ **ผู้เล่น** โดยเน้นหัวข้อที่อาจารย์กำหนดใน README Format ได้แก่ Overview, Rules และ Game Manual

---

## 1. Overview

**Energy Not Included** เป็นเกมแนว **Turn-based Resource Management / Strategy** ที่ผู้เล่นรับบทเป็นเจ้าของบริษัทพลังงาน แข่งขันกันสำรวจทรัพยากรธรรมชาติ สร้างโรงงานผลิตทรัพยากร สร้างโรงไฟฟ้า และเชื่อมต่อระบบส่งพลังงานเพื่อขายไฟฟ้าให้เมืองต่าง ๆ บนแผนที่

เกมจะเล่นแบบผลัดกันเดิน ผู้เล่นแต่ละคนต้องบริหาร **Coin**, **Action Point**, **Resource**, **Power Plant**, **Link**, **City Contract** และ **Waste** ให้ดีที่สุด เมื่อเมืองทั้งหมดได้รับไฟฟ้าครบ เกมจะจบ และระบบจะตัดสินผู้ชนะจากคะแนนรวมของผู้เล่นแต่ละคน

### ข้อมูลเกม

| หัวข้อ | รายละเอียด |
|---|---|
| ชื่อเกม | Energy Not Included |
| แนวเกม | Turn-based Resource Management / Strategy |
| จำนวนผู้เล่น | 2-3 คน |
| Platform | Web Browser / Localhost |
| Front-end | HTML, CSS, JavaScript |
| Back-end | C++ |
| Library | Mongoose C Library สำหรับ HTTP/WebSocket server |
| เป้าหมายหลัก | จ่ายไฟให้เมืองและทำคะแนนรวมให้สูงที่สุด |

---

## 2. How to Play

# 2.1 Rules

## เป้าหมายของเกม

ผู้เล่นต้องสร้างระบบพลังงานตั้งแต่ต้นทางจนถึงปลายทาง ได้แก่

1. สร้าง **Resource Plant** เพื่อผลิตทรัพยากร
2. สร้าง **Power Plant** เพื่อผลิตไฟฟ้า
3. สร้าง **Link** เพื่อเชื่อม Resource Node, Power Plant Node และ City Node
4. ส่งไฟฟ้าให้เมืองจนเมืองได้รับพลังงานเพียงพอ
5. ทำคะแนนรวมให้สูงกว่าผู้เล่นคนอื่นเมื่อเกมจบ

เกมจบเมื่อ **ทุกเมืองในแผนที่ได้รับไฟฟ้าแล้ว** หรือเมื่อเหลือผู้เล่นที่ยังไม่แพ้เพียงคนเดียว

---

## จำนวนผู้เล่นและเทิร์น

- เกมรองรับผู้เล่น **2-3 คน**
- ผู้เล่นเล่นแบบ **Turn-based** คือผลัดกันเล่นทีละคน
- ใน 1 เทิร์น ผู้เล่นมี **3 Action Points**
- 1 Action ใช้ **1 Action Point**
- Action ส่วนใหญ่ต้องใช้ **Coin** ด้วย
- ถ้า Action Point หมด ระบบจะเปลี่ยนไปผู้เล่นถัดไป
- ถ้าผู้เล่นไม่อยากทำ Action ต่อ สามารถกด **Next Turn** เพื่อข้ามเทิร์นได้

---

## ทรัพยากรหลักในเกม

| สิ่งที่ต้องบริหาร | ความหมาย |
|---|---|
| Coin | เงินของผู้เล่น ใช้สร้างสิ่งปลูกสร้าง และนับเป็นคะแนนตอนจบเกม |
| Action Point | แต้มสำหรับทำ Action ในแต่ละเทิร์น มี 3 แต้มต่อเทิร์น |
| Resource | วัตถุดิบ เช่น Coal, Gas, Biomass, Uranium ใช้ผลิตไฟฟ้า |
| Energy | ไฟฟ้าที่ผลิตจาก Power Plant และส่งให้เมือง |
| Waste | ของเสียจากการผลิต ทำให้คะแนนลดลง และถ้ามากเกินไปอาจแพ้ |

---

## ประเภท Node บนแผนที่

| Node | หน้าที่ |
|---|---|
| Resource Plant Node | ใช้สร้าง Resource Plant เพื่อผลิตทรัพยากร |
| Power Plant Node | ใช้สร้าง Power Plant เพื่อผลิตไฟฟ้า |
| Small City Node | เมืองขนาดเล็ก รับไฟฟ้าและให้คะแนนเมือง 75 คะแนน |
| Big City Node | เมืองขนาดใหญ่ รับไฟฟ้าและให้คะแนนเมือง 150 คะแนน |
| Empty Node | ช่องว่าง ไม่ใช่จุดสร้างอาคารหลัก |

---

## ประเภท Resource

Resource ในเกมมี 4 ประเภท

| Resource | ใช้ทำอะไร |
|---|---|
| Coal | ใช้กับ Coal Plant |
| Gas | ใช้กับ Gas Plant |
| Biomass | ใช้กับ Biomass Plant |
| Uranium | ใช้กับ Nuclear Plant |

---

## ประเภท Power Plant

| Power Plant | ประเภทการผลิต | เงื่อนไขหลัก |
|---|---|---|
| Coal Plant | Resource-based | ต้องรับ Coal จาก Resource Plant |
| Gas Plant | Resource-based | ต้องรับ Gas จาก Resource Plant |
| Biomass Plant | Resource-based | ต้องรับ Biomass จาก Resource Plant |
| Nuclear Plant | Resource-based | ต้องรับ Uranium จาก Resource Plant |
| Solar Plant | Passive | ใช้ค่า solar_index ของ Power Plant Node |
| Wind Plant | Passive | ใช้ค่า wind_index ของ Power Plant Node |
| Hydro Plant | Passive | ใช้ค่า has_water ของ Power Plant Node |

หมายเหตุ: โรงไฟฟ้าแบบ Passive ไม่จำเป็นต้องรับ Resource จาก Resource Plant แต่ใช้ค่าพื้นที่ของ Node แทน เช่น Solar ใช้ solar_index และ Wind ใช้ wind_index

---

## กติกาการสร้าง Link

Link ใช้เชื่อม Node เพื่อส่งของระหว่างกัน โดยมีเส้นทางหลัก 2 แบบ

### 1. Resource Node → Power Plant Node

ใช้ส่ง Resource จาก Resource Plant ไปยัง Power Plant

ตัวอย่าง:

```txt
Coal Resource Plant → Coal Plant
Gas Resource Plant → Gas Plant
Biomass Resource Plant → Biomass Plant
Uranium Resource Plant → Nuclear Plant
```

### 2. Power Plant Node → City Node

ใช้ส่ง Energy จาก Power Plant ไปยังเมือง

ตัวอย่าง:

```txt
Coal Plant → Small City
Solar Plant → Big City
Nuclear Plant → Big City
```

ถ้าเชื่อม Node ผิดประเภท หรือ Node ไม่มี Building ที่จำเป็น การส่ง Resource/Energy อาจไม่เกิดผล

---

## เงื่อนไขการจบเกม

เกมจะจบเมื่อเกิดเงื่อนไขใดเงื่อนไขหนึ่งต่อไปนี้

1. **เมืองทุกเมืองได้รับไฟฟ้าแล้ว**
2. **เหลือผู้เล่นที่ยังไม่แพ้เพียง 1 คน**

---

## เงื่อนไขการแพ้

ผู้เล่นจะแพ้หรือถูกตัดออกจากเกมได้ถ้า

- Coin ไม่พอจ่ายค่าใช้จ่ายสำคัญหรือเข้าสู่สถานะล้มละลาย
- Waste มากกว่าหรือเท่ากับ 100

---

## การคิดคะแนนตอนจบเกม

เมื่อเกมจบ ระบบจะคำนวณคะแนนรวมจากทรัพย์สินและผลกระทบของผู้เล่น

| ประเภทคะแนน | วิธีคิดคะแนน | คำอธิบาย |
|---|---:|---|
| Coin | 1 Coin = 1 คะแนน | Coin ที่เหลือทั้งหมดนำมาคิดคะแนน |
| Power Plant | 75% ของราคาสร้าง | โรงไฟฟ้าที่สร้างไว้มีมูลค่าเป็นคะแนน |
| Resource Plant | 70% ของราคาสร้าง | โรงงานทรัพยากรที่สร้างไว้มีมูลค่าเป็นคะแนน |
| Small City | 75 คะแนน | ได้จากเมืองเล็กที่ผู้เล่นจ่ายไฟสำเร็จ |
| Big City | 150 คะแนน | ได้จากเมืองใหญ่ที่ผู้เล่นจ่ายไฟสำเร็จ |
| Waste | -2 คะแนนต่อ 1 Waste | Waste เป็นค่าติดลบ ยิ่งมีมากคะแนนยิ่งลด |

ผู้เล่นที่มี **คะแนนรวมสูงที่สุด** เป็นผู้ชนะ

---

# 2.2 Game Manual

## ขั้นตอนที่ 1: เริ่มเกม

1. เปิดเกมผ่าน Browser
2. เข้าไปที่หน้าเริ่มเกม `index.html` หรือ `http://localhost:18888`
3. เลือกจำนวนผู้เล่นด้วยปุ่ม `+` หรือ `-`
4. เลือกแผนที่ เช่น `The Rust Factory`
5. เลือกระดับความยาก เช่น `Beginner`
6. กดปุ่ม `INITIALIZE MISSION`
7. ระบบจะส่งคำสั่งเริ่มเกมไปยัง Back-end
8. เมื่อเริ่มเกมสำเร็จ ระบบจะพาไปหน้า Gameplay

---

## ขั้นตอนที่ 2: ทำความเข้าใจกระดาน

กระดานเกมเป็นแผนที่แบบ 2D Grid มีข้อมูลซ้อนกัน 2 ชั้น

| Layer | ความหมาย |
|---|---|
| Node Layer | ชั้นล่าง แสดงประเภท Node เช่น Resource, Power, City |
| Building Layer | ชั้นบน แสดงสิ่งปลูกสร้างของผู้เล่น เช่น Resource Plant หรือ Power Plant |

ผู้เล่นต้องเลือกช่องให้ถูกประเภทก่อนสร้างสิ่งปลูกสร้าง เช่น Resource Plant ต้องสร้างบน Resource Node เท่านั้น และ Power Plant ต้องสร้างบน Power Plant Node เท่านั้น

---

## ขั้นตอนที่ 3: ลำดับการเล่นใน 1 เทิร์น

ในแต่ละเทิร์นให้เล่นตามลำดับนี้

1. ดูว่าเป็นเทิร์นของผู้เล่นคนใด
2. ตรวจสอบ Coin และ Action Point ที่เหลือ
3. เลือก Action ที่ต้องการทำ
4. กรอกหรือเลือก Input ของ Action นั้น
5. กดสร้างหรือยืนยัน Action
6. ระบบจะหัก 1 Action Point และหัก Coin ตาม Cost
7. ทำ Action ต่อได้จน Action Point หมด
8. เมื่อไม่ต้องการทำต่อ ให้กด Next Turn

---

## Action 1: Create Resource Plant

ใช้สร้างโรงงานผลิตทรัพยากรบน Resource Node

### Input ที่ใช้

| Input | คำอธิบาย |
|---|---|
| ตำแหน่ง Node | ตำแหน่งช่องที่ต้องการสร้าง เช่น row/column หรือ x/y |
| Resource Type | Coal, Gas, Biomass หรือ Uranium |
| Player | ผู้เล่นเจ้าของโรงงาน |

### เงื่อนไข

- ต้องเลือก Resource Node
- Node ต้องยังไม่มี Building
- ผู้เล่นต้องมี Action Point อย่างน้อย 1
- ผู้เล่นต้องมี Coin เพียงพอ

### ผลลัพธ์

- สร้าง Resource Plant บน Node
- ใช้ 1 Action Point
- หัก Coin ตามราคาสร้าง
- Resource Plant จะผลิต Resource และ Waste ตามชนิดทรัพยากร

---

## Action 2: Create Power Plant

ใช้สร้างโรงไฟฟ้าบน Power Plant Node

### Input ที่ใช้

| Input | คำอธิบาย |
|---|---|
| ตำแหน่ง Node | ตำแหน่ง Power Plant Node ที่ต้องการสร้าง |
| Power Plant Type | Coal, Gas, Biomass, Nuclear, Solar, Wind หรือ Hydro |
| Player | ผู้เล่นเจ้าของโรงไฟฟ้า |

### เงื่อนไข

- ต้องเลือก Power Plant Node
- Node ต้องยังไม่มี Building
- ผู้เล่นต้องมี Action Point อย่างน้อย 1
- ผู้เล่นต้องมี Coin เพียงพอ
- ถ้าเป็น Solar/Wind/Hydro ควรเลือก Node ที่มีค่าแวดล้อมเหมาะสม

### ผลลัพธ์

- สร้าง Power Plant บน Node
- ใช้ 1 Action Point
- หัก Coin ตามราคาสร้าง
- Power Plant จะผลิต Energy เมื่อมี Input เพียงพอ

---

## Action 3: Create Link

ใช้สร้างเส้นเชื่อมระหว่าง Node เพื่อส่ง Resource หรือ Energy

### Input ที่ใช้

| Input | คำอธิบาย |
|---|---|
| Node A | Node ต้นทาง |
| Node B | Node ปลายทาง |
| Link Type / Transfer Type | Resource หรือ Energy ตามชนิดการเชื่อม |
| Player | ผู้เล่นที่สร้าง Link |

### รูปแบบ Link ที่ถูกต้อง

| จาก | ไป | สิ่งที่ส่ง |
|---|---|---|
| Resource Node | Power Plant Node | Resource |
| Power Plant Node | City Node | Energy |

### เงื่อนไข

- ต้องเลือก Node 2 จุด
- ควรเป็น Node ที่มี Building แล้ว
- ต้องเชื่อมตามทิศทางการผลิตที่ถูกต้อง
- ผู้เล่นต้องมี Action Point อย่างน้อย 1
- ผู้เล่นต้องมี Coin เพียงพอ

### ผลลัพธ์

- สร้าง Link ระหว่าง Node
- ใช้ 1 Action Point
- ส่ง Resource หรือ Energy ตามประเภท Node
- ถ้าเมืองได้รับ Energy เพียงพอ เมืองจะถูกนับว่าได้รับไฟฟ้าแล้ว

---

## Action 4: Next Turn / Skip Turn

ใช้จบเทิร์นของตัวเองเมื่อไม่ต้องการทำ Action ต่อ

### Input ที่ใช้

| Input | คำอธิบาย |
|---|---|
| ปุ่ม Next Turn | ใช้ข้ามไปผู้เล่นถัดไป |

### ผลลัพธ์

- จบเทิร์นของผู้เล่นปัจจุบัน
- รีเซ็ต Action Point เมื่อกลับมาถึงเทิร์นของผู้เล่นอีกครั้ง
- เปลี่ยนไปยังผู้เล่นถัดไป

---

## ตัวอย่าง Flow การเล่น

```txt
Player 1 Turn
1. Create Resource Plant บน Coal Resource Node
2. Create Power Plant เป็น Coal Plant บน Power Plant Node
3. Create Link จาก Coal Resource Node ไป Coal Plant
Next Turn

Player 2 Turn
1. Create Resource Plant บน Gas Resource Node
2. Create Power Plant เป็น Gas Plant
3. Create Link จาก Gas Plant ไป Small City
Next Turn
```

จากนั้นผู้เล่นแต่ละคนต้องค่อย ๆ สร้างเครือข่ายพลังงานให้เมืองได้รับไฟฟ้าให้ครบ และพยายามเก็บคะแนนให้สูงกว่าคู่แข่ง

---

## Checklist สำหรับผู้เล่นใหม่

ก่อนเริ่มเล่นควรรู้สิ่งเหล่านี้

- ต้องสร้าง Resource Plant บน Resource Node เท่านั้น
- ต้องสร้าง Power Plant บน Power Plant Node เท่านั้น
- Resource Plant ส่ง Resource ไป Power Plant
- Power Plant ส่ง Energy ไป City
- 1 เทิร์นมี 3 Action Points
- Waste ทำให้คะแนนลดลง
- เกมจบเมื่อเมืองทุกเมืองได้รับไฟฟ้า
- คะแนนสูงสุดเป็นผู้ชนะ
