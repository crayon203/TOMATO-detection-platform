import argparse
import os
from ultralytics import YOLO

parser = argparse.ArgumentParser()
parser.add_argument("--model", required=True, help="model path (.pt or .onnx)")
parser.add_argument("--img",   required=True, help="image path or directory")
parser.add_argument("--conf",  type=float, default=0.25, help="confidence threshold")
parser.add_argument("--iou",   type=float, default=0.45, help="IoU threshold")
parser.add_argument("--batch", action="store_true", help="batch mode: img is a directory")
parser.add_argument("--names", default="", help="comma-separated class names (overrides model built-in names)")
opt = parser.parse_args()

# 构建类别名称覆盖表
custom_names = {}
if opt.names:
    for i, name in enumerate(opt.names.split(",")):
        custom_names[i] = name.strip()

# 收集图片列表
valid_exts = ('.jpg', '.jpeg', '.png', '.bmp', '.tiff', '.webp', '.tif')
if opt.batch and os.path.isdir(opt.img):
    image_files = sorted([
        os.path.join(opt.img, f) for f in os.listdir(opt.img)
        if f.lower().endswith(valid_exts)
    ])
else:
    image_files = [opt.img]

model = YOLO(opt.model)

for img_path in image_files:
    if not os.path.exists(img_path):
        print(f"skip: {img_path}")
        continue

    results = model.predict(img_path, conf=opt.conf, iou=opt.iou, verbose=False)

    for r in results:
        boxes = r.boxes
        if boxes is None:
            continue

        for i in range(len(boxes)):
            x1, y1, x2, y2 = map(int, boxes.xyxy[i])
            w = x2 - x1
            h = y2 - y1
            conf = float(boxes.conf[i])
            cls_id = int(boxes.cls[i]) if boxes.cls is not None else 0

            if conf < 0.01:
                continue

            # 优先用自定义名称，否则用模型自带的
            if cls_id in custom_names:
                cls_name = custom_names[cls_id]
            elif hasattr(model, 'names') and cls_id in model.names:
                cls_name = model.names[cls_id]
            else:
                cls_name = "tomato"

            print(x1, y1, w, h, conf, cls_id, cls_name)
