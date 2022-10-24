<div id = "header" align = "center">
  <img src = "https://avatars.githubusercontent.com/u/95591065?v=4" width = 90/>
  <div id="badges">
  <a href="https://www.linkedin.com/in/calin-basturea-349a15234/">
    <img src="https://img.shields.io/badge/LinkedIn-blue?style=for-the-badge&logo=linkedin&logoColor=white" alt="LinkedIn Badge"/>
  </a>
  </div>
</div>

# A CLI Web Scrapper that displays data about stocks/crypto currency entered from the keyboard

This is my first attempt at creating a web scrapper! It takes about one second or less to look up each stock/crypto currency.

Run the program by using the existing Makefile, either by compiling it yourself.
```cpp
g++ main.cpp -lcurl -o curl
```

 You can change the program's language(atm only Romanian and English are available) by changing "EN" to "RO" in the <code><b>program_settings.json</b></code> file.
 

 
 ![image](https://user-images.githubusercontent.com/95591065/197540432-7d78081e-a2b5-4de6-b984-28943678c46b.png)
 
<b> (currency hasn't been implemented yet) </b>

<h1>Running the Program</h1>

![image](https://user-images.githubusercontent.com/95591065/197541958-34ae4ce8-3f50-4d2a-b219-3d70632bcd71.png)
![image](https://user-images.githubusercontent.com/95591065/197542210-8415a72c-c5ae-47f2-8d41-42df04cb6512.png)

The json file <code>pref_stocks.json / pref_crypto.json</code> keeps track of each stock/crypto currency and the number of times you've searched for that stock/crypto currency.

![image](https://user-images.githubusercontent.com/95591065/197542464-ce40f28e-4f08-4e27-a050-b6bdfa4d08f8.png)
