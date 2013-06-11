StackMob Analytics
=====================================

## Overview

StackMob automatically aggregates your API calls to your datastore and <a href="https://marketplace.stackmob.com/module/customcode" target="_blank">Custom Code</a> methods.  In doing so, we're able to provide analytics views for you.

<a href="https://dashboard.stackmob.com/module/analytics/api" target="_blank">View your Analytics</a>

<a href="https://dashboard.stackmob.com/module/analytics/api" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/analytics/modules-analytics-apicalls.png" alt=""/></a>

## Types of Analytics

You have access to:

* Datastore Calls
* Custom Code Calls
* Push Calls

You can also view different metrics:

### Calls, Errors, and more

View number of API calls you've made.
View average response times (in milliseconds)
View amount of data transferred (in KB)
View number of errors (409 conflicts, 404 not found, etc)

<img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/analytics/modules-analytics-metric-types.png" alt=""/>

### Schemas and Custom Code

View data for your schemas and also custom code.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/analytics/modules-analytics-data-types.png" alt=""/>

### HTTP Verb Types

See if people have read, created, updated, or deleted schemas.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/analytics/modules-analytics-http-verbs.png" alt=""/>

## Separate Data between Development and Production

Since StackMob gives you two environments, Development and Production, naturally your analytics for the two differ.  Choose that here.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/analytics/modules-analytics-environments.png" alt=""/>



<p class="alert">
	<b>Tip:</b> make some objects you create or update solely for the purpose of analytics tracking.  For example if you're tracking click outs on a shopping cart, perhaps you can create a <code>ShoppingCartClick</code> schema and simply make "empty" update calls to it when somebody clicks out on the shopping cart.  You can then query for number of <code>PUT</code> calls for <code>ShoppingCartClick</code>
</p>