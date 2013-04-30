# Setting up Custom Domains

StackMob allows you to setup custom domain names.  If you have access to change your domain's CNAME on your domain name service, then you can have you domain point to StackMob's hosted HTML5 service.

When your HTML5 is hosted on StackMob, you can easily use <a href="http://developer.stackmob.com/tutorials/js">StackMob's JS SDK</a> to access StackMob's backend services.

# Step 1 of 2. Registering your domain with StackMob

To have `www.yourapp.com` be a StackMob-hosted site, fill in your <a href="https://dashboard.stackmob.com/module/html5/settings" target="_blank">Manage HTML5: Domains</a> page like below:

<div><a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_addingcustomdomain.png" /></a>
</div>

# Step 2 of 2. Adding CNAME records

After saving, proceed to your domain registrar and add a CNAME entry for StackMob.  The subdomain value should be * and it should point to stackmobapp.com. If your provider does not support wildcard DNS records, you'll need to create one CNAME record using the development prefix you entered on the Manage HTML5 page and another with the subdomain "www" (both pointing to stackmobapp.com.)

That's it!  http://dev.yourapp.com and http://www.yourapp.com are now served at StackMob.

# Root Level Domain Names (aka Naked Domains)

Unfortunately, naked domains and cloud services don't play well together. Because an A record must point to a static IP address, it limits our ability to make dynamic changes to improve security, scalability, and performance. There's a good blog post describing the dangers and limits in terms of scalability that using A records for cloud services here: http://neilmiddleton.com/the-dangers-of-a-records-and-heroku/

What we (and many other services) recommend is redirecting your naked domain to the www subdomain (or any subdomain of your choice.)

if you use DNSimple you can use their ALIAS feature http://blog.dnsimple.com/zone-apex-naked-domain-alias-that-works/
