StackMob CORS
=====================================

At StackMob, we want to make your mobile development life as simple as possible while providing you with the most powerful set of tools to help you along the way. For those of you using our JavaScript SDK, we are happy to announce support for <a href="http://en.wikipedia.org/wiki/Cross-origin_resource_sharing" title="CORS" target="_blank">Cross-origin Resource Sharing (CORS)</a>.  Additionally, <a href="https://developer.stackmob.com/sdk" target="_blank">JavaScript SDK v0.8.0</a> has also been released to utilize CORS support in your apps.

<h3>What?</h3>

As a security measure, web browsers block JavaScript from making calls between different domains.  This is known as the <a href="https://developer.mozilla.org/en-US/docs/JavaScript/Same_origin_policy_for_JavaScript" target="_blank">Same Origin Policy</a>.  CORS allows you to make these cross domain calls and access your StackMob data from a list of hostnames that you approve.

<h3>Why?</h3>

Previously, to access the StackMob API from a non-StackMob domain (such as a local development server or a custom domain), you were required to run your app through the StackMob web server or an API proxy.  The python web server was used to proxy API requests to StackMob and circumvent the same-origin policy within web browsers. Now with the <a href="https://dashboard.stackmob.com/module/cors/settings" target="_blank">CORS module</a>, you can specify a list of domains that are permitted to execute CORS requests against your development and production StackMob API's. For many, this will simplify development setup so you can easily use any web server you want; but the provided python server still works if that's easiest for you. It will also open new doors to running StackMob JavaScript from new locations such as cloud IDEs and your own domains.  For detailed upgrade notes, check the <a href="https://github.com/stackmob/stackmob-js-sdk/blob/master/CHANGELOG.md" target="_blank">changelog</a>.

<h3>Compatibility</h3>
CORS is compatibile with most modern desktop and mobile browsers. <a href="http://caniuse.com/cors" target="_blank">Click here for a full list of compatible browsers</a>.  If you need to support browsers that are not CORS-compatible, don't worry. If you host your app on StackMob's <a href="https://marketplace.stackmob.com/module/html5" target="_blank">HTML5 Hosting Module</a>, the JavaScript SDK will run in non-CORS mode.

<h3>Setup</h3>
To add your own domains or development hosts, <a href="https://dashboard.stackmob.com/module/cors/settings" target="_blank">head over to the CORS module in your dashboard</a>. 

<a href="https://dashboard.stackmob.com/module/cors/settings" target="_blank"><img src="http://blog-static.stackmob.com.s3.amazonaws.com/wordpress/wp-content/uploads/2013/02/Screen-Shot-2013-02-27-at-6.11.34-PM.png" alt="CORS Domains Editor" width="798" height="405" class="aligncenter size-full wp-image-16171" /></a>

<a href="https://dashboard.stackmob.com/signup" class="btn btn-success">Sign Up</a> 
<a href="https://dashboard.stackmob.com/module/cors" class="btn btn-success">Configure your CORS settings</a>