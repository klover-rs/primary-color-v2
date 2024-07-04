const fs = require('fs');
const https = require('https');
const url = require('url');
const AdmZip = require('adm-zip');
const path = require('path');
const { exec } = require('child_process');

const platform = process.platform;
const arch = process.arch;


let product;

function determine_product() {

    console.log(arch);
    console.log(platform);
    switch (`${arch}-${platform}`) {
        case "arm64-linux":
            product = "aarch64-unknown-linux-gnu.a";
            break;
        case "x64-win32":
            product = "x86_64-pc-windows-msvc.zip";
            break;
        case "arm64-darwin":
            product = "x64_86-apple-darwin.a";
            break;
        case "x64-darwin":
            product = "x64_86-apple-darwin.a";
            break;
        default:
            product = "undefined";
            break;
    }
}

const download = (fileUrl, dest, callback) => {
    const file = fs.createWriteStream(dest);
  
    const request = https.get(fileUrl, (response) => {
      if (response.statusCode === 302) {
        const newUrl = response.headers.location;
        console.log(`Redirected to: ${newUrl}`);
        download(newUrl, dest, callback);
      } else if (response.statusCode !== 200) {
        callback(new Error(`Failed to get '${fileUrl}' (${response.statusCode})`));
      } else {
        response.pipe(file);
        file.on('finish', () => {
          file.close((err) => {
            if (err) {
                callback(err);
            } else {
                console.log('Download completed!');
                callback(null);
            }
          });
        });
      }
    });
  
    request.on('error', (err) => {
      fs.unlink(dest, () => {}); 
      console.error(`Error downloading the file: ${err.message}`);
    });
};

const buildNodeGypProject = (projectPath, callback) => {
    const options = { cwd: projectPath };

    exec('node-gyp configure', options, (configureError, configureStdout, configureStderr) => {
        if (configureError) {
            console.error('Error configuring project:', configureError);
            console.error('stderr:', configureStderr);
            callback(configureError);
            return;
        }

        console.log('Configure output:', configureStdout);

        exec('node-gyp build', options, (buildError, buildStdout, buildStderr) => {
            if (buildError) {
                console.error('Error building project:', buildError);
                console.error('stderr:', buildStderr);
                callback(buildError);
                return;
            }

            console.log('Build output:', buildStdout);
            callback(null, buildStdout);
        });
    });
};

function extractZip(filePath, outputDir, callback) {
    try {
        const zip = new AdmZip(filePath);
        zip.extractAllTo(outputDir, true);
        console.log('File successfully extracted to', outputDir);
        callback(null); // No error
    } catch (err) {
        console.error('An error occurred while unzipping:', err);
        callback(err); // Pass the error to the callback
    }
}


function main() {
    determine_product();

    const githuburl = `https://github.com/mari-rs/primary-color-v2/releases/download/1.0.0/${product}`;

    const projectPath = path.join(__dirname, '..');
    const destPath = path.join(__dirname, product);
    const libPath = path.join(__dirname, '..', 'lib');

    download(githuburl, destPath, (err) => {
        if (err) {
            console.error("AN ERROR:" + err);
        } else {
            
            console.log('Binary downloaded successfully.');

            console.log("LIB PATH: " + libPath);
    
            if (!fs.existsSync(libPath)) {
                fs.mkdirSync(libPath, { recursive: true });
            }

            switch (product) {
                case "x86_64-pc-windows-msvc.zip":
                    const zipFilePath = path.join(__dirname, product);
                    const outputDir = path.join(__dirname, 'output');
                    const build_dir = path.join(__dirname, "..", "build", "Release");
        
                    extractZip(zipFilePath, outputDir, (err) => {
                        if (err) {
                            console.error('Extraction failed:', err);
                        } else {
                            console.log('Extraction succeeded!');
                            if (!fs.existsSync(build_dir)) {
                                fs.mkdirSync(build_dir, { recursive: true });
                            }
                            const output_folder = path.join(__dirname, "output"); 
        
                            fs.renameSync(path.join(output_folder, "primary_image_color.dll.lib"), path.join(libPath, "primary_image_color.dll.lib"));
                            fs.renameSync(path.join(output_folder, "primary_image_color.dll"), path.join(build_dir, "primary_image_color.dll"));

                            buildNodeGypProject(projectPath, (err, result) => {
                                if (err) {
                                    console.error('Build failed:', err);
                                    fs.rmSync(outputDir, {recursive: true});
                                    fs.rmSync(destPath, {recursive: true});
                                    process.exit(0);
                                } else {
                                    console.log('Build succeeded:', result);
                                    fs.rmSync(outputDir, {recursive: true});
                                    fs.rmSync(destPath, {recursive: true});
                                    process.exit(0);
                                }
                            });


                        }
                    });
                    break;
                case "aarch64-unknown-linux-gnu.a":
                    fs.renameSync(destPath, path.join(libPath, "libprimary_image_color.a"));
                    buildNodeGypProject(projectPath, (err, result) => {
                        if (err) {
                            console.error('Build failed:', err);
            
                            process.exit(0);
                        } else {
                            console.log('Build succeeded:', result);
                            
                            process.exit(0);
                        }
                    });


                    break;
                case "aarch64-apple-darwin.a":
                    fs.renameSync(
                      destPath,
                      path.join(libPath, "libprimary_image_color.a"),
                    );
              
                    buildNodeGypProject(projectPath, (err, result) => {
                        if (err) {
                            console.error("Build failed:", err);
                            process.exit(0);
                        } else {
                            console.log("Build succeeded:", result);
                            process.exit(0);
                        }
                    });
                    break;
                case "x86_64-apple-darwin.a":
                    fs.renameSync(
                        destPath,
                        path.join(libPath, "libprimary_image_color.a"),
                    );
              
                    buildNodeGypProject(projectPath, (err, result) => {
                        if (err) {
                            console.error("Build failed:", err);
                            process.exit(0);
                        } else {
                            console.log("Build succeeded:", result);
                            process.exit(0);
                        }
                    });
                    break;
                default:
                    console.log("unsupported :(");
            }
    
        }
    });

    

   
    
}

main();