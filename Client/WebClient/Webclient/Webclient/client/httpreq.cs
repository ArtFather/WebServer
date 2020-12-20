using System;
using System.Net.Http;

namespace Webclient.client
{
    public class Httpreq
    {
        static readonly HttpClient client = new HttpClient();
        string str = ""; 
        public async void HttpReq(string ur)
        {
            try
            {
                Console.WriteLine(ur);
                HttpResponseMessage response = await client.GetAsync("http://" + ur);
                response.EnsureSuccessStatusCode();
                string responseBody = await response.Content.ReadAsStringAsync();
                str = responseBody;
                return new 
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine("Exception Caught! Message :{0}");
                Console.WriteLine(ex.Message);
            }
            catch (UriFormatException ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public string ReturnJson()
        {
            return str;
        }

        public Httpreq Astr(Httpreq ex)
        {
            Httpreq obj = new Httpreq();
            obj.str = str + ex.str;
            return obj;
        }
    }
}
