const express = require('express')
const { IpVPN } = require('./build/Release/vpn')
const app = express()
console.log(process.env.D)
const vvv = new IpVPN(process.env.D)

app.get('/', (req, res) => {
  console.time('S')
  const rs = vvv.isVPN('0.0.0.0')
  console.timeEnd('S')
  res.status(200).send(rs)
})
app.listen(3001)
