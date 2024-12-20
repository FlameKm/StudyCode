# Design Pattern

## Explanation

| 设计模式     | 描述                                                         | 目的                       | 举例                                                         |
| :----------- | ------------------------------------------------------------ | -------------------------- | ------------------------------------------------------------ |
| 命令模式     | 将请求封装为一个对象，使得请求可以被参数化、排队、记录日志、或者支持可撤销操作。请求的发送者和接收者解耦，使用者只需知道命令接口，而不需了解具体的实现细节。 | 请求的封装和解耦           | 一个按键，管理者负责连接功能（开灯，关灯），使用者只需要按下按键 |
| 中介者模式   | 通过引入一个中介者对象来减少多个对象之间的直接通信，所有的交互都通过中介者进行，从而降低对象之间的耦合度。 | 减小对象间的交互           | 一个公司类，包含两个同事和聊天工具，同事只需负责发送到聊天软件和接收聊天工具的数据 |
| 代理模式     | 为其他对象提供一个代理以控制对这个对象的访问。代理可以在访问之前或之后添加额外的功能，例如权限控制、延迟加载、日志记录等。 | 侧重于访问、权限控制       | 书店卖书，创建一个网络代理书店，两者存在卖书的接口           |
| 桥接模式     | 将抽象部分与其实现部分分离。                                 | 抽象与实现的解耦           | 相机 有 RGB 和 TOF 相机，都有帧率设置接口，但是具体的通信控制不一样 |
| 单例模式     | 确保一个类只有一个实例，并提供一个全局访问点。               | 确保对象唯一性             | 比如日至管理，只需要一个日至管理器                           |
| 工厂模式     | 专门定义**一个类**来负责创建其它类的实例，被创建的实例通常都具有共同的父类 | 灵活性                     | 工厂生产，不是直接生产水果，通过对应的果林进行生产才可获取水果 |
| 原型模式     | 对象提供一个 clone 接口，允许自我复制                        | 避免直接拷贝，涉及深浅拷贝 | 图片可以自我复制                                             |
| 备忘录模式   | 在不暴露对象实现细节的情况下，捕获一个对象的内部状态，并在需要时将对象恢复到之前的状态。 | 实现状态的保存和恢复       | 一个文本编辑器，可以保存和恢复文本内容的历史状态，用户可以通过撤销操作返回到之前的文本状态 |
| 策略模式     | 准备一组算法,并将每一个算法封装起来,使得它们可以互换；       | 灵活性                     | 计算器，传入不同的计算方式，得出计算结果                     |
| 建造者模式   | 一个对象的构建比较复杂，将一个对象的构建和对象的表示进行分离 | 简化复杂对象的创建         | 建房子，可以自由选择门、窗、墙壁                             |
| 抽象工厂模式 | 提供一个接口，用于创建**一系列**相关或相互依赖的对象，而无需指定它们具体的类 | 实现对象的解耦和灵活性     | 工厂生产，不是直接生产水果，通过果林进行生产才可获取水果，一个果林可以生产多种水果 |
| 装饰模式     | 动态的给一个对象添加一些额外的职责；侧重于功能的扩展：对其功能可以自由的增加或者减少； | 功能的扩展                 | 游戏角色，动态获取技能;<br />不同操作增加不同的属性          |
| 适配器模式   | 适配器是将2个不同的对象进行连接和组合！将一个类的接口转换成客户希望的另外一个接口 | 兼容性                     | 只有 220 V 交流电压，通过适配器类转换为 12 V 直流电源        |
| 迭代器模式   | 提供一种方法，顺序访问一个聚合对象中的元素，而不暴露该对象的内部表示。 | 集合对象的遍历更加灵活     | 书架上放着许多书籍，通过迭代器，我们可以方便地逐本阅读;<br />C 中链表遍历 |
| 模板模式     | 提前将业务逻辑做好了，就是同一种模板；                       |                            | 还是建房子，只需一键 build， build 会放置 门 窗 墙壁         |

