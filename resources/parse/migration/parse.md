[**UPDATE : Parse Migration tool is available for download.**](https://github.com/stackmob/stackmob-parse-migrator)

We’ve built a [migration tool](https://github.com/stackmob/stackmob-parse-migrator), but if you prefer to do it manually, you can import your Parse data by using our REST API.

<div><ol><li>From the Parse dashboard, click the “Settings” button for your app.</li>
<li>From the left navigation, click “Export Data” and then press the “Export Data” button.</li>
<li>You will be emailed a zip file which will contain a .json file for each of your classes.</li>
<li>For the sake of this exercise, let’s assume you have a class called “rating”.  Open the zip file and remove <code>{ "results":</code> from the top of rating.json, as well as the final <code>}</code>.
<div class="row">
<div class="span5">
<b>Before</b>
<pre><code class="brush: js;">
{ "results": [
{
  "Place": "Taqueria Dos Amigos",
  "comment": "try the baby burrito",
  "rating": 3,
  "createdAt": "2013-04-26T00:50:49.752Z",
  "updatedAt": "2013-04-26T00:51:09.200Z",
  "objectId": "0wyAuWlQaS"
},
...
] }
</pre></code>
</div>
<div class="span5">
<b>After</b>
<pre><code class="brush: js;">
[
{
  "Place": "Taqueria Dos Amigos",
  "comment": "try the baby burrito",
  "rating": 3,
  "createdAt": "2013-04-26T00:50:49.752Z",
  "updatedAt": "2013-04-26T00:51:09.200Z",
  "objectId": "0wyAuWlQaS"
},
...
]
</pre></code>
</div>
</div>
</li>
<li>Install oauth-proxy.  If you’re a mac user, just type <code>easy_install oauth-proxy</code>. <a href="https://stackmob.com/devcenter/docs/Calling-the-REST-API-with-cURL" target="_blank">Details on using curl with our REST API</a></li>
<li>Run oauth-proxy (if you're logged in, we'll prepopulate the following with your real app keys!):
<div class="dynamicMarkdown">
<pre><code class="brush: js;">
oauth-proxy --consumer-key "YOUR PUBLIC KEY" --consumer-secret "YOUR PRIVATE KEY"
</code></pre>
</div>
You can get your public and private key from <a href="https://dashboard.stackmob.com/settings" target="_blank">the StackMob Dashboard</a>.
</li>  
<li>Run this command: <pre><code class="brush: js;">curl -svx localhost:8001 -H "Accept: application/vnd.stackmob+json; version=0" -d @rating.json http://api.stackmob.com/rating.json</code></pre></li>
<li>Go to the <a href="https://dashboard.stackmob.com/data/browser" target="_blank">StackMob Object Browser</a> and you will see your results!</li></ol></div>
