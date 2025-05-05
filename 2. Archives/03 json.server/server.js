const http = require('http');
const fs = require('fs');
const path = require('path');

const filePath = path.join(__dirname, 'data.json'); // JSON file path

const server = http.createServer((req, res) => {

  if (req.method === 'GET') {
    // Handle GET request
    fs.readFile(filePath, 'utf8', (err, data) => {
      if (err) {
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Failed to read JSON file' }));
      } else {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(data); // Send JSON content
      }
    });
  } else if (req.method === 'POST') {
    // Handle POST request
    let body = '';
    req.on('data', chunk => {
      body += chunk.toString(); // Collect incoming data
    });

    req.on('end', () => {
      // Parse the new data from the POST request
      let newPost;
      try {
        newPost = JSON.parse(body); // Parse the incoming JSON data
      } catch (error) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Invalid JSON format' }));
        return;
      }

      // Read the current data from the JSON file
      fs.readFile(filePath, 'utf8', (err, data) => {
        if (err && err.code !== 'ENOENT') {
          res.writeHead(500, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ error: 'Failed to read JSON file' }));
          return;
        }

        // If file is empty or malformed, set jsonData to an empty array
        let jsonData = [];
        if (data) {
          try {
            jsonData = JSON.parse(data); // Try parsing the existing data
            if (!Array.isArray(jsonData)) { // If it's not an array, reset to empty array
              jsonData = [];
            }
          } catch (error) {
            jsonData = []; // If parsing fails, reset to an empty array
          }
        }

        // Append the new post to the existing data
        jsonData.push(newPost);

        // Write the updated data back to the JSON file
        fs.writeFile(filePath, JSON.stringify(jsonData, null, 2), 'utf8', err => {
          if (err) {
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Failed to write to JSON file' }));
          } else {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Data appended successfully' }));
          }
        });
      });
    });
  } else {
    // Handle unsupported methods
    res.writeHead(405, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Method not allowed' }));
  }
});

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}/`);
});
