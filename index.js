const imeeji = require('bindings')('addon')
const fs = require('fs')

const imageFileName = 'image1.bmp'
const buffer = fs.readFileSync(`./images/${imageFileName}`)
imeeji.invert(`./images/${imageFileName}`)
// fs.writeFileSync(`./images/negative-${imageFileName}`, newBuffer)
