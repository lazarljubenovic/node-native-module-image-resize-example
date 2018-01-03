const imeeji = require('bindings')('addon')
const fs = require('fs')

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
  imeeji.invert(imageFileName)
})
