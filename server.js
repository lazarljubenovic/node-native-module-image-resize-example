const express = require('express')
const fs = require('fs')
const path = require('path')
const app = express()

const base64 = {}

function walk(folder, fn) {
  fs.readdirSync(folder).forEach(item => {
    // console.log(folder, item)
    const stat = fs.lstatSync(path.join(folder, item))
    if (stat.isDirectory(stat)) {
      walk(item)
    } else if (stat.isFile(stat)) {
      fn(item)
    } else {
      console.error(`Taj tvoj Linux!`)
    }
  })
}

walk(path.join(__dirname, './unsplash'), fileName => {
  const thumbnailPath = path.join(__dirname, './resized', fileName)
  const thumbnail = fs.readFileSync(thumbnailPath)
  const thumbnailBase64 = new Buffer(thumbnail).toString('base64')
  base64[fileName] = thumbnailBase64
})

app.get('/', (req, res) => {
  let html = ''
  Object.keys(base64).forEach(fileName => {
    const fileBase64 = base64[fileName]
    html += `<img style="background-image: url(data:image/jpg;base64,${fileBase64}); background-size: cover; background-repeat: no-repeat; background-position: center center"
                  src="/images/${fileName}"
                  width="300"
             >`
  })
  res.send(html)
})

app.use('/thumbnails', express.static('resized'))
app.use('/images', express.static('unsplash'))
// app.get('/image/:name', (req, res) => {
  //
// })

app.listen(3000, () => console.log('Listening on port 3000'))
