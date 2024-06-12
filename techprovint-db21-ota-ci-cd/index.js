const path = require('path')
const semver = require('semver')
const { Storage } = require('@google-cloud/storage')
const { BigQuery } = require('@google-cloud/bigquery')

const BQ_DATASET = 'ota'
const BQ_TABLE = 'firmwares'
const TABLE_SCHEMA = 'id:string, eventType: string, bucket:string, version:string, fullname:string, filename:string, variant:string, createdAt:timestamp, latestVersion:boolean, env:string'
const projectId = process.env.GCLOUD_PROJECT

const bqClient = new BigQuery({ projectId })
const storage = new Storage()
const bucket = storage.bucket(`${projectId}-firmware`)

var devices = {};
// devices['a4ca9912cfa4'] = "dev";
// devices['a0b19a12cfa4'] = "dev";
// devices['346edd988330']  = "dev";
// devices['446edd988330']  = "dev";
// devices['b86edd988330']  = "dev";
// devices['c86edd988330']  = "dev";
// devices['406edd988330']  = "dev";
// devices['446edd988330']  = "dev";
// devices['7c10e9accd98'] = "prod";
// devices['8810e9accd98'] = "prod";
// devices['c86edd988330'] = "prod";
// devices['c46edd988330'] = "prod";
// devices['506edd988330'] = "prod";
// devices['346edd988330'] = "prod";
devices['4010e9accd98']  = "prod";
devices['1011e9accd98']  = "prod";
devices['2810e9accd98']  = "prod";
devices['8810e9accd98']  = "prod";
devices['606edd988330']  = "prod";
devices['506edd988330']  = "dev";   // 2.1 device with Tharanga


function getDeviceEnvironment(deviceId) {
  return devices[deviceId]
}

async function insertIntoBigquery(data) {
  const [datasets] = await bqClient.getDatasets()
  let dataset = datasets.find(dataset => dataset.id === BQ_DATASET)
  console.log('Datasets', datasets, dataset)
  // let dataset = bqClient.dataset( BQ_DATASET )
  if (!dataset) {
    [dataset] = await bqClient.createDataset(BQ_DATASET)
  }

  const [tables] = await dataset.getTables()
  let table = tables.find(table => table.id === BQ_TABLE)
  console.log('Tables', tables, table)
  // let table = dataset.table( BQ_TABLE )    
  if (!table) {
    [table] = await dataset.createTable(BQ_TABLE, { schema: TABLE_SCHEMA })
  }

  try {
    const res = await table.insert(data, { ignoreUnknownValues: true });
    console.log('Insert', res)
  } catch (e) {
    console.log('Insert Failed', e, JSON.stringify(e))
  }
}

async function getPublicUrl(filename) {
  console.log("filename : ", filename);
  const file = bucket.file(filename)
  console.log("file : ", file);
  await file.makePublic()
  const url = `http://${bucket.name}.storage.googleapis.com/${filename}`
  console.log("url : ", url);
  return url
}

/**
 * Generic background Cloud Function to be triggered by Cloud Storage.
 *
 * @param {object} data The event payload.
 * @param {object} context The event metadata.
 */
exports.insertFirmwaresOnBigquery = (data, context) => {
  const file = data;

  const filename = path.basename(file.name)
  const ext = path.extname(filename)
  if (ext !== '.bin') {
    console.log('Not a firmware file.')
    return 'ok'
  }

  const filenameWithoutExt = path.basename(file.name, ext)
  const dir = path.dirname(file.name)
  const version = dir.split('/').pop()
  const variant = filenameWithoutExt.split('_').pop()
  const env = 'dev'
  const latestVersion = true
  const row = {
    id: context.eventId,
    bucket: file.bucket,
    version,
    fullname: file.name,
    filename,
    variant,
    createdAt: file.timeCreated,
    latestVersion: latestVersion,
    env: env,
    eventType: context.eventType,
  }

  console.log('Row to insert', row)
  return insertIntoBigquery(row)
};

/**
 * Responds to any HTTP request.
 * @param {!express:Request} req HTTP request context.
 * @param {!express:Response} res HTTP response context.
 */
exports.getDownloadUrl = async (req, res) => {
  try {
    const { version, variant, devid } = req.query
    console.log('Fetch version and variant ', version, variant)

    const env = getDeviceEnvironment(devid);
    if (env == '' || env == null) {
      res.status(200).send("Device not found: " + devid);
    }
    console.log("Environment: ", env)
    const queryParams = {
      query: `
        SELECT 
          bucket,
          fullname,
          version,
          createdAt
        FROM \`divolgo-jenkins.ota.firmwares\`
        where variant = @variant and
        latestVersion = true and 
        env = @env
        order by createdAt desc
        limit 1      
      `,
      params: {
        variant,
        env
      }
    }

    const [rows] = await bqClient.dataset(BQ_DATASET).table(BQ_TABLE).query(queryParams)

    if (rows.length > 0) {
      const firmware = rows[0]

      // latest > current
      const needsUpdate = semver.gt(firmware.version, version)
      console.log("needsUpdate : New firmware : ", needsUpdate, firmware.version);

      if (needsUpdate) {
        const url = await getPublicUrl(firmware.fullname)
        console.log('Sending url', url)
        res.status(200).send(url);
      } else {
        res.status(204).send('Up to date');
      }

    } else {
      res.status(204).send('No new version');
    }

  } catch (err) {
    res.status(500).send(err.message);
  }
}
