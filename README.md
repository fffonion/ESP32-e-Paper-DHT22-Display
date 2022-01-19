# ESP32-e-Paper-DHT22-Display

读取温湿度，并显示在墨水屏上。
基于 TTGO T5-display 和 DHT22。

- 可局部刷新，间歇性全刷新防烧屏
- 可选开启通过 statsd 发送数据
- AirKiss 自动配网
    - “乐鑫信息科技”官方公众号 -> 商铺 -> AirKiss 设备
    - 按住左按键，绿灯亮后不松开重新配置
- 24小时历史图
- 支持 OTA 更新
    - 按住左按键，绿灯亮后松开进入 OTA 模式
- 复制 `src/config.example.h` 到 `src/config.h` 配置 OTA 密码和静态IP地址


![](https://user-images.githubusercontent.com/2135445/150112236-b0d2758a-e382-4a10-9eae-a35d272719b1.gif)




