# RaftKVDatabase

## 基于raft协议的高可用kv内存型数据库

### 使用
在config/config.json文件中配置集群信息并启动集群

* 支持GET,SET,DELETE操作
* ```GET key\r\n```来获取相应key的value
* ```SET key value\r\n```来设置key的value
* ```DELETE key\r\n```来删除相应的key
* 集群之间会自动转发命令到leader,并返回结果
