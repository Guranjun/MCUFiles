import serial
import sys
import signal
import os

# ================== 配置 ==================
PORT = 'COM3'                # 串口端口
BAUDRATE = 921600            # 波特率
OUTPUT_BIN = 'latest.jpg'    # 输出的 JPEG 二进制文件
OUTPUT_HEX = 'latest_jpeg_data.txt'  # 输出的十六进制文本文件
BYTES_PER_LINE = 16          # 十六进制文本每行显示的字节数

# ================== 辅助函数 ==================
def save_jpeg(jpeg_data):
    """
    将 JPEG 二进制数据保存为图像文件，同时生成十六进制文本文件。
    """
    # 保存二进制图像
    with open(OUTPUT_BIN, 'wb') as f:
        f.write(jpeg_data)
    print(f"\n已保存图像：{OUTPUT_BIN}，共 {len(jpeg_data)} 字节")

    # 保存十六进制文本（每行 BYTES_PER_LINE 字节）
    with open(OUTPUT_HEX, 'w', encoding='utf-8') as f:
        for i in range(0, len(jpeg_data), BYTES_PER_LINE):
            line = ' '.join(f'{b:02X}' for b in jpeg_data[i:i+BYTES_PER_LINE])
            f.write(line + '\n')
    print(f"已保存十六进制文本：{OUTPUT_HEX}")

def find_sequence(buffer, seq):
    """在字节数组中查找序列 seq 第一次出现的位置，返回索引，未找到返回 -1"""
    for i in range(len(buffer) - len(seq) + 1):
        if buffer[i:i+len(seq)] == seq:
            return i
    return -1

# ================== 主程序 ==================
def main():
    # 打开串口
    try:
        ser = serial.Serial(
            port=PORT,
            baudrate=BAUDRATE,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )
        print(f"成功打开串口 {PORT}，波特率 {BAUDRATE}")
        print("正在监听数据，检测到 FF D8 开始记录，检测到 FF D9 保存图像...")
        print("按 Ctrl+C 退出程序\n")
    except serial.SerialException as e:
        print(f"打开串口失败：{e}")
        sys.exit(1)

    # JPEG 起始和结束标志
    JPEG_START = bytes([0xFF, 0xD8])
    JPEG_END   = bytes([0xFF, 0xD9])

    buffer = bytearray()          # 数据缓冲区
    total_received = 0            # 总计接收字节数（仅用于显示）

    # 设置信号处理，实现优雅退出
    def signal_handler(sig, frame):
        print("\n正在关闭串口...")
        ser.close()
        sys.exit(0)
    signal.signal(signal.SIGINT, signal_handler)

    try:
        while True:
            # 读取当前所有可用数据
            if ser.in_waiting > 0:
                data = ser.read(ser.in_waiting)
                buffer.extend(data)
                total_received += len(data)

                # 打印接收进度（每 1KB 更新一次）
                if total_received % 1024 == 0:
                    print(f"已接收 {total_received} 字节", end='\r')

                # 处理缓冲区中的数据，提取 JPEG
                while True:
                    # 查找起始标志
                    start_idx = find_sequence(buffer, JPEG_START)
                    if start_idx == -1:
                        # 没有找到起始标志，丢弃缓冲区所有数据（因为没有开始）
                        buffer.clear()
                        break

                    # 如果起始标志之前有数据，丢弃它们（保留从 start_idx 开始的数据）
                    if start_idx > 0:
                        buffer = buffer[start_idx:]

                    # 现在缓冲区以 JPEG_START 开头，查找结束标志
                    end_idx = find_sequence(buffer, JPEG_END)
                    if end_idx == -1:
                        # 未找到结束标志，等待更多数据
                        break

                    # 找到结束标志，提取完整 JPEG 数据（包含起始和结束标志）
                    jpeg_data = buffer[:end_idx + len(JPEG_END)]
                    # 保存文件（覆盖）
                    save_jpeg(jpeg_data)
                    # 移除已处理的 JPEG 数据，继续处理后面的数据
                    buffer = buffer[end_idx + len(JPEG_END):]
                    # 循环处理，可能缓冲区中还有另一幅图像
            else:
                # 没有数据时短暂休眠，避免 CPU 占用过高
                import time
                time.sleep(0.001)
    except Exception as e:
        print(f"发生错误：{e}")
    finally:
        ser.close()
        print(f"\n串口已关闭，共接收 {total_received} 字节")

if __name__ == "__main__":
    main()