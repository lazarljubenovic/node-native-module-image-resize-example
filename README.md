# node-native-module-image-resize-example

This is a simple example of using native modules written in C++ with help of [NAN](https://github.com/nodejs/nan).
The created module exports a single function with the following signature.

```ts
type Resizer = (imageFileName: string, imageDimensions: number) => void
```

It reads an image based on the given relative path, and resizes it to a square, using a simple averaging method.
Then it saves the resized image as an image with the same name in sibling folder `resized`.

The idea is to use these smaller images as thumbnails during implementation of
[LQIP technique for lazy loading images](https://blog.imgix.com/2016/06/01/lqip-your-images.html).

![Visualisation of resizing a large image to 3x3 thumbnail](https://i.imgur.com/U7EKy3f.png)

---

To run, you need `node` and `npm`. Development has been done in Node 9.3.0 and npm 5.4.1, but it should work in
other newer versions as well.

```
npm i            # will compile the module as well
mkdir resized    # create folder where resized images will be placed
mkdir unsplash   # folder with images
cd unsplash      # we have to be in the folder with images
node ./index.js  # will create images and print success/fail messages to standard output
cd ..            # back to project root
node server.js   # a simple express server tha demonstrates the effect
```

The effect should look something like the following:

![Example in action](https://i.imgur.com/XyCVedY.png)
