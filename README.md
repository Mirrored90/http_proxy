This is a HTTP proxy which function with GET, POST, CONNECT.
Proxy has a LRU cache of size 800, which can caching user's frequently used response.
Our proxy uses port 12345. To start our proxy, please first cd to docker-deploy,
and run "sudo docker-compose build", then run "sudo docker-compose up".