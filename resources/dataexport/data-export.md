Export StackMob Data
=====================================

## Overview

StackMob enables you to [export](https://dashboard.stackmob.com/data/export) the data from both your development and production environments.

This includes:
* API data 
* Push tokens
* Facebook/Twitter credentials

Due to security concerns, user passwords and Facebook/Twitter access tokens are not exported.

## API Data Format

From the [data export page](https://dashboard.stackmob.com/data/export), you can select between JSON or CSV formats.

Note that the id/primary key associated with each schema is annotated as "_id" (rather than the custom primary key alias). If an id was not explicitly provided, a new id would be automaticlly generated with an "$oid" annotation (e.g. <code>_id" : { "$oid" : "1fe27a8fed39384d87865e56fe9" }</code>).  
