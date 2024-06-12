PROJECT_ID=divolgo-jenkins
gcloud config set project $PROJECT_ID
gcloud functions deploy insertFirmwaresOnBigquery --runtime nodejs8 --trigger-resource $PROJECT_ID-firmware --trigger-event google.storage.object.finalize
gcloud functions deploy getDownloadUrl --runtime nodejs8 --trigger-http