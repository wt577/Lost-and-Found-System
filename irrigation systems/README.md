# &nbsp;					智能浇灌系统

# \*\*Smart Agricultural Multi-Cloud Low-Power Irrigation System (IWS)\*\*

# 

# !\[MCU](https://img.shields.io/badge/MCU-STM32F103-blue.svg) !\[Power](https://img.shields.io/badge/Power-Ultra%20Low%20Sleep-green.svg) !\[Cloud](https://img.shields.io/badge/IoT-AliCloud%20%7C%20OneNET-orange.svg) !\[Protocol](https://img.shields.io/badge/Protocol-MQTT%20%7C%20AT-lightgrey.svg)

# 

# 本项目是一个面向现代化精准农业的\*\*多云协同低功耗物联网控制系统\*\*。系统以 STM32F103 为核心，通过软硬件深度耦合优化，实现了对农田环境（温湿度、光照）的高精度动态采集与水泵的精准远程控制。特别针对野外农业场景，设计了\*\*低功耗睡眠唤醒机制\*\*与\*\*边缘端数据防丢失策略\*\*。

# 

# ---

# 

# \##  核心特性 (Core Features)

# 

# \- \*\*极低功耗架构 (Low Power Design)\*\*：深度应用 STM32 睡眠模式（Sleep Mode），配合定时器与外部中断（EXTI）事件驱动唤醒机制。在非采集/通信周期内自动休眠，\*\*系统整体待机功耗降低 18%\*\*，大幅延长野外部署的电池寿命。

# \- \*\*异构多云平台接入 (Multi-Cloud IoT)\*\*：抽象了底层网络透传模块，通过自主封装的 USART AT 指令集与 MQTT 协议栈，实现同一套代码可动态切换/兼容接入 \*\*阿里云、OneNET、原子云\*\* 三大主流物联网平台。

# \- \*\*边缘端状态持久化 (EEPROM Storage)\*\*：结合 I2C 协议开发 EEPROM 掉电存储驱动，自动保存云端下发的浇灌阈值、定时任务与设备运行状态，确保设备断电重启后业务无缝恢复。

# \- \*\*高精度采集与执行 (High Precision Control)\*\*：

#   - \*\*采集端\*\*：基于 SPI/I2C 协议与 ADC 配合滤波算法，实现温湿度、光照等环境参数的平滑采集，数据误差严格控制在 \*\*≤3%\*\*。

#   - \*\*执行端\*\*：采用高频 PWM 驱动电机/水泵，结合网络延迟补偿算法，使云端远程下发的浇灌时长控制精度达到 \*\*±0.5s\*\*。

# 

# ---

# 

# \## 系统架构设计 (System Architecture)

# 

# ```text

# \[感知与执行层]                  \[边缘控制层: STM32F103]                 \[云端与应用层]

#  

#  (光照/温湿度)                       +-----------------+                      \[阿里云]

#   I2C / SPI   ----------+----------->|   ADC / EXTI    |                        ^

#                         |            |                 |                        |

#  (土壤湿度)             |            |    EEPROM驱动   |                      \[OneNET]

#   ADC 模拟量  ----------+            |    (掉电保护)   |                        ^

#                                      |                 |    USART / MQTT        |

#  (水泵/电机)  <--------- PWM --------|  睡眠功耗管理   |----------------> \[4G/WiFi DTU]

#                                      +-----------------+                        |

#                                                                                 v

#                                                                             \[原子云]

