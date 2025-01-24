```CSharp
// 改变RectTransform的top和Right
GetComponent<RectTransform>().offsetMax = new Vector2(left, top);
// bottom left
rectTransform.offsetMin = new Vector2(right, bottom);
// width height
rectTransform.sizeDelta = new Vector2(width, height);
// pos
rectTransform.anchoredPosition3D = new Vector3(posx,posy,posz);
rectTransform.anchoredPosition = new Vector2(posx,posy);
```