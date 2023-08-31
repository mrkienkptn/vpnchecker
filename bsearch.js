const readline = require('readline')
const fs = require('fs')

class VPN {
  /**
   * @param {string} filePath
   */
  constructor (filePath) {
    this.ips = []
    this.filePath = filePath
    this.readFile()
  }
  comparePairIps (p1, p2) {
    if (p2[0] > p1[0]) return true
    return false
  }
  readFile () {
    if (!this.filePath || !fs.existsSync(this.filePath)) {
      throw new Error('Data file path does not exist')
    }
    const x1 = process.memoryUsage().heapUsed / 1024 / 1024
    const content = fs.readFileSync(this.filePath, 'utf-8')
    const x2 = process.memoryUsage().heapUsed / 1024 / 1024
    content.split(/\r?\n/).forEach(line => {
      const [start, end] = line.split(',')
      if (start && end) {
        this.ips.push([
          Number(start.substring(1, start.length - 1)),
          Number(end.substring(1, end.length - 1))
        ])
      }
    })
    console.log(x2-x1)
    this.ips.sort(this.comparePairIps)
    const used = process.memoryUsage().heapUsed / 1024 / 1024
    console.log(
      `The script uses approximately ${Math.round(used * 100) / 100} MB`
    )
  }

  isVPN (ip) {
    let left = 0
    let right = this.ips.length
    let near = -1
    while (left <= right) {
      let mid = Math.floor(left + (right - left) / 2)
      if (this.ips[mid][0] <= ip) {
        near = mid
        left = near + 1
      } else {
        right = mid - 1
      }
    }
    if (near === -1) return false
    if (ip <= this.ips[near][1]) return true
    return false
  }
}

const a = new VPN('IP2PROXY-LITE-PX1.CSV')
// console.log(a.isVPN(3758094285))

