/*
You can use this file to convert an HTML file to a format that can be used in an ESP32 project.
// TIP: Edit the `index.html` file to include the HTML code you want to convert, then run this file with Node.js or Nodemon: `node convert-html-to-esp32.js` or `nodemon convert-html-to-esp32.js`
*/

const fs = require('fs')
const readline = require('readline')

const rl = readline.createInterface({
  input: fs.createReadStream('index.html'),
  output: process.stdout,
  terminal: false,
})

// Read the file line by line
rl.on('line', (line) => {
  // Trim the line to remove whitespace from the beginning and end
  const trimmedLine = line.trim()

  // Skip processing if the line is blank
  if (trimmedLine === '') {
    return
  }

  // Escape double quotes in the trimmed line
  const escapedLine = trimmedLine.replace(/"/g, '\\"')

  // Output the escaped line in a format suitable for C++
  console.log(`client.println("${escapedLine}");`)
})

rl.on('close', () => console.log('\nFinished reading file'))
