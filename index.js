const express = require('express')
const { IpVPN } = require('./build/Release/test')
const app = express()

const vvv = new IpVPN('IP2PROXY-LITE-PX1.CSV')

app.get('/', (req, res) => {
  console.time('S')
  const rs = vvv.isVPN('0.0.0.0')
  console.timeEnd('S')
  res.status(200).send(rs)
})
app.listen(3001)
