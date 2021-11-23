const chokidar = require('chokidar');
const postcss = require('postcss');
const fs = require('fs');
const path = require('path');
const stylelint = require('stylelint');
const reporter = require('postcss-reporter');

const htdocsRoot = path.join(__dirname, '..', '..', '..', '..');
const stylelintConfigPath = path.join(htdocsRoot, 'web/core/.stylelintrc.json');
const inputFilePattern = path.join(__dirname, 'assets/css/*.pcss.css');
const outputDir = path.join(__dirname, 'assets/css/');

const stylelintConfig = JSON.parse(fs.readFileSync(stylelintConfigPath));

const processFile = (file) => {
  console.log(`Processing file: ${file}`);
  fs.readFile(file, (err, css) => {
    if (err) throw err;

    postcss()
      .use(stylelint(stylelintConfig))
      .use(reporter())
      .process(css, { from: file, to: file.replace('.pcss.css', '.css') })
      .then((result) => {
        fs.writeFile(file.replace('.pcss.css', '.css'), result.css, () => true);
        if (result.map) {
          fs.writeFile(file.replace('.pcss.css', '.css.map'), result.map, () => true);
        }
        console.log(`Processed file: ${file}`);
      })
      .catch((error) => {
        console.error(`Error processing file: ${file}`, error);
      });
  });
};

const watcher = chokidar.watch(inputFilePattern, {
  usePolling: true,
  interval: 1000,
});

watcher.on('all', (event, filePath) => {
  console.log(`Event: ${event}, Path: ${filePath}`);
  if (event === 'change') {
    processFile(filePath);
  }
});

console.log('Watching for changes...');
