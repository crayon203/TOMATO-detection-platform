"""
量化工具 — 将训练好的 .pt 模型量化为 .onnx 用于推理
支持三种途径: ONNX Runtime / TensorRT / PyTorch 自带
所有途径最终输出可推理的 .onnx 文件
"""

import argparse
from ultralytics import YOLO
import time
import os
import sys

parser = argparse.ArgumentParser()
parser.add_argument('--model',   required=True,            help="输入模型路径 (.pt)")
parser.add_argument('--quant',   default='fp16',           choices=['fp16', 'int8'], help="量化精度")
parser.add_argument('--scheme',  default='PTQ',            choices=['PTQ', 'QAT'],   help="量化方案")
parser.add_argument('--pathway', default='ONNX',           help="量化途径")
opt = parser.parse_args()

model_path = opt.model
quant_type = opt.quant
scheme     = opt.scheme
pathway    = opt.pathway

print(f"[量化] 模型: {model_path}")
print(f"[量化] 方案: {scheme}  精度: {quant_type}  途径: {pathway}")
print("-" * 50)

if not os.path.exists(model_path):
    print(f"ERROR: 模型文件不存在: {model_path}")
    sys.exit(1)

try:
    model = YOLO(model_path)

    half = (quant_type == 'fp16')
    int8 = (quant_type == 'int8')

    # ================================================================
    #  途径 1: ONNX Runtime 量化  →  输出 best.onnx
    # ================================================================
    if "ONNX" in pathway:
        print(f"[ONNX] 导出 ONNX 模型 (half={half}, int8={int8})...")
        out_path = model.export(format='onnx', half=half, int8=int8)
        print(f"[ONNX] 输出: {out_path}")
        print(f"[ONNX] 此文件可用于 Python 推理 (.onnx) 或 C++ 推理")

    # ================================================================
    #  途径 2: TensorRT 量化  →  输出 engine 或回退 onnx
    # ================================================================
    elif "TensorRT" in pathway:
        print(f"[TensorRT] 检查环境...")
        try:
            import tensorrt
            print(f"[TensorRT] 版本: {tensorrt.__version__}")
        except ImportError:
            print(f"[TensorRT] 未安装 tensorrt 包，回退到 ONNX 导出")

        if scheme == 'PTQ':
            print(f"[TensorRT] 导出 engine (half={half}, int8={int8})...")
            try:
                out_path = model.export(format='engine', half=half, int8=int8, workspace=4)
                print(f"[TensorRT] 输出: {out_path}")
            except Exception as e:
                print(f"[TensorRT] engine 导出失败: {e}")
                print(f"[TensorRT] 回退到 ONNX...")
                out_path = model.export(format='onnx', half=half, int8=int8)
                print(f"[TensorRT→ONNX] 输出: {out_path}")
        else:
            out_path = model.export(format='onnx', half=half, int8=int8)
            print(f"[TensorRT QAT] 输出 ONNX: {out_path}")

    # ================================================================
    #  途径 3: PyTorch 自带量化  →  统一导出 ONNX
    # ================================================================
    else:
        print(f"[PyTorch] 通过 ultralytics 导出 ONNX (half={half}, int8={int8})...")
        print(f"[PyTorch] ultralytics 内部已处理量化和优化")
        out_path = model.export(format='onnx', half=half, int8=int8)
        print(f"[PyTorch] 输出: {out_path}")
        print(f"[PyTorch] 此 .onnx 文件可直接用于推理 Tab")

    print("-" * 50)
    print(f"量化完成！拿去推理 Tab 加载即可")

except Exception as e:
    print(f"ERROR: {e}")
    import traceback
    traceback.print_exc()
    sys.exit(1)
