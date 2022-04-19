
### RSA命令行生成
文档：http://t.zoukankan.com/274914765qq-p-4665959.html
mkdir -p test/keys
openssl genrsa -aes128 -passout pass:"123456" -out rsa_private_key.pem 3072
openssl rsa -in rsa_private_key.pem -pubout -out rsa_public_key.pem

### 也可能使用hb库中程序生成，结果都能识别