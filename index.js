const resizer = require('bindings')('addon')
const fs = require('fs')

const THUMB_SIZE = 4

const folder = process.cwd()
// const folder = __dirname

function walk(folder, fn) {
  fs.readdirSync(folder).forEach(item => {
    const stat = fs.lstatSync(item)
    if (stat.isDirectory(stat)) {
      walk(item)
    } else if (stat.isFile(stat)) {
      fn(item)
    } else {
      console.error(`Taj tvoj Linux!`)
    }
  })
}

walk(folder, (imageFileName) => {
  resizer.invert(imageFileName, THUMB_SIZE)
})
