from ultralytics import YOLO
import argparse
import os
import torch
import sys
import json
import time


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument("--data", type=str, required=True)
    parser.add_argument("--epochs", type=int, default=50)
    parser.add_argument("--batch", type=int, default=16)
    parser.add_argument("--imgsz", type=int, default=640)
    parser.add_argument("--device", type=str, default="cpu")
    # 新增参数
    parser.add_argument("--lr", type=float, default=0.001)
    parser.add_argument("--optimizer", type=str, default="AdamW")
    parser.add_argument("--weight_decay", type=float, default=0.0005)
    parser.add_argument("--val_split", type=float, default=0.2)

    args = parser.parse_args()

    print("====================================")
    print("🚀 YOLOv8 Training Start")
    print("📂 Dataset:", args.data)
    print("📊 Epochs:", args.epochs)
    print("📦 Batch:", args.batch)
    print("🖼 Img Size:", args.imgsz)
    print("🧠 Device:", args.device)
    print("📐 Learning Rate:", args.lr)
    print("⚙️ Optimizer:", args.optimizer)
    print("🔧 Weight Decay:", args.weight_decay)
    print("✂️ Val Split:", args.val_split)
    print("====================================")

    # ==============================
    # 数据集检查
    # ==============================
    if not os.path.exists(args.data):
        print("❌ 数据集不存在:", args.data)
        sys.exit(1)

    # ==============================
    # 自动设备选择
    # ==============================
    if args.device.lower() in ["0", "gpu"]:
        if torch.cuda.is_available():
            device = 0
            print("✅ 使用 GPU")
        else:
            device = "cpu"
            print("⚠️ 无GPU，自动切换 CPU")
    else:
        device = "cpu"
        print("✅ 使用 CPU")

    # ==============================
    # 权重路径
    # ==============================
    script_dir = os.path.dirname(os.path.abspath(__file__))
    weights_path = os.path.join(script_dir, "yolov8n.pt")

    if not os.path.exists(weights_path):
        print("❌ 权重文件不存在:", weights_path)
        sys.exit(1)

    print("📦 权重加载:", weights_path)

    try:
        model = YOLO(weights_path)
        start_time = time.time()

        # ==============================
        # 开始训练
        # ==============================
        results = model.train(
            data=args.data,
            epochs=args.epochs,
            batch=args.batch,
            imgsz=args.imgsz,
            device=device,
            lr0=args.lr,
            optimizer=args.optimizer,
            weight_decay=args.weight_decay,
            fraction=1.0 - args.val_split,  # 训练集比例
            verbose=False
        )

        train_time = time.time() - start_time
        minutes = int(train_time // 60)
        seconds = int(train_time % 60)

        print("====================================")
        print("🎉 Training Finished")
        print("⏱ 训练耗时: {}分{}秒".format(minutes, seconds))
        print("====================================")

        # ==============================
        # 输出 JSON 格式训练指标（供 Qt 解析）
        # ==============================
        metrics = {
            "status": "success",
            "train_time": "{}分{}秒".format(minutes, seconds),
            "train_time_seconds": round(train_time, 1),
            "model_path": "",
            "map50": 0.0,
            "map50_95": 0.0,
            "precision": 0.0,
            "recall": 0.0,
        }

        # 尝试获取最佳模型路径
        try:
            if hasattr(results, 'save_dir'):
                best_pt = os.path.join(str(results.save_dir), "weights", "best.pt")
                if os.path.exists(best_pt):
                    metrics["model_path"] = best_pt
        except:
            pass

        # 尝试从 results 获取指标
        try:
            if hasattr(results, 'results_dict'):
                d = results.results_dict
                metrics["map50"] = round(float(d.get("metrics/mAP50(B)", 0)), 4)
                metrics["map50_95"] = round(float(d.get("metrics/mAP50-95(B)", 0)), 4)
                metrics["precision"] = round(float(d.get("metrics/precision(B)", 0)), 4)
                metrics["recall"] = round(float(d.get("metrics/recall(B)", 0)), 4)
        except Exception as e:
            print("⚠️ 获取指标异常:", str(e))

        # 尝试从 saved_csv 或 best.pt 目录获取
        if metrics["map50"] == 0.0:
            try:
                csv_path = os.path.join(str(results.save_dir), "results.csv")
                if os.path.exists(csv_path):
                    with open(csv_path) as f:
                        lines = f.readlines()
                        if len(lines) > 1:
                            last = lines[-1].strip().split(",")
                            # YOLO CSV: epoch,train/box_loss,...metrics/mAP50(B),metrics/mAP50-95(B),...
                            for i, col in enumerate(lines[0].strip().split(",")):
                                if "mAP50(B)" in col and i < len(last):
                                    metrics["map50"] = round(float(last[i]), 4)
                                if "mAP50-95(B)" in col and i < len(last):
                                    metrics["map50_95"] = round(float(last[i]), 4)
            except Exception as e:
                print("⚠️ CSV解析异常:", str(e))

        # 输出 JSON（一行，方便 C++ 解析）
        print("@@METRICS_JSON@@")
        print(json.dumps(metrics, ensure_ascii=False))
        print("@@END_METRICS@@")

    except Exception as e:
        print("❌ 训练失败：", str(e))
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()
