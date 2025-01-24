```CSharp
public class Network : MonoBehaviour
{
    void Start()
    {
        StartCoroutine(Get());
        StartCoroutine(Post());
    }
    IEnumerator Get()
    {
        UnityWebRequest webRequest = UnityWebRequest.Get("http://www.baidu.com");
        yield return webRequest.SendWebRequest();
        if (webRequest.isHttpError || webRequest.isNetworkError)
            Debug.Log(webRequest.error);
        else
            Debug.Log(webRequest.downloadHandler.text);
    }
    IEnumerator Post()
    {
        WWWForm form = new WWWForm();
        form.AddField("key", "value");
        form.AddField("name", "mafanwei");
        form.AddField("blog","qwe25878");
        UnityWebRequest webRequest = UnityWebRequest.Post("http://www.baidu.com", form);
        yield return webRequest.SendWebRequest();
        if (webRequest.isHttpError || webRequest.isNetworkError)
            Debug.Log(webRequest.error);
        else
            Debug.Log(webRequest.downloadHandler.text);
    }
}
```