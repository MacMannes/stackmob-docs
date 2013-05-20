<h1>StackMob Custom Domains</h1>

# Setting up Custom Domains

StackMob allows you to setup custom domain names.  If you have access to change your domain's CNAME on your domain name service, then you can have you domain point to StackMob's hosted HTML5 service.

When your HTML5 is hosted on StackMob, you can easily use [StackMob's JS SDK](http://developer.stackmob.com/tutorials/js) to access StackMob's backend services.

# Step 1 of 2. Registering your domain with StackMob

To have `www.yourapp.com` be a StackMob-hosted site, fill in your [Manage HTML5: Domains](https://dashboard.stackmob.com/module/html5/settings) page like below:

<div><a class="screenshot" href="https://dashboard.stackmob.com/module/html5/settings" target="_blank"><img src="//s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_HTML_addingcustomdomain.png" /></a>
</div>

# Step 2 of 2. Adding CNAME records

After saving, proceed to your domain registrar and add a CNAME entry for StackMob.  The subdomain value should be * and it should point to `stackmobapp.com.` If your provider does not support wildcard DNS records, you'll need to create one CNAME record using the development prefix you entered on the Manage HTML5 page and another with the subdomain `www` (both pointing to `stackmobapp.com.`)

That's it! `http://dev.yourapp.com` and `http://www.yourapp.com` are now served at StackMob.

# Root Level Domain Names (aka Naked or Bare Domains)

Unfortunately, root-level domains and cloud services don't play well together. Because an A record must point to a static IP address, it limits our ability to make dynamic changes to improve security, scalability, and performance. There's [a good blog post by Neil Middleton](http://neilmiddleton.com/the-dangers-of-a-records-and-heroku/) describing the dangers and limits of using A records, in terms of scalability for cloud services.

We and [others](https://devcenter.heroku.com/articles/apex-domains) recommend the following: in your DNS provider's configuration, redirect your root-level domain to your `www` subdomain (or any subdomain of your choice) using an A record, then point your `www` subdomain to `stackmobapp.com.` with a CNAME record as described above. Then, create an entry for `www.yourdomain.com` in your Custom Domain configuration, and both the naked and `www` domains should work.

Users of DNSimple can also use their [ALIAS feature](http://blog.dnsimple.com/zone-apex-naked-domain-alias-that-works/).

It is important to configure your DNS A record properly, otherwise the naked domain may not resolve. This table summarizes what will happen to a request without an A record set:

<table border="1">
<tr>
<th></th>
<th>Visit www.yourapp.com</th>
<th>Visit yourapp.com</th>
</tr>
<tr>
<td><b>Naked domain in<br />Custom Domain settings</b></td>
<td>OK</td>
<td>OK</td>
</tr>
<tr>
<td><b>www domain in<br />Custom Domain settings</b></td>
<td>OK</td>
<td>404</td>
</tr>
</table>

Setting an A record as above resolves the 404 case. So, set an A record!



# Using SSL (HTTPS) with Custom Domains - HTML5

### Objective

Create a secure, SSL-enabled custom domain for an HTML5 application.

In this, we will show you how to enable SSL (HTTPS) for your HTML5 application, allowing you to encrypt all the traffic
that is sent between your users and StackMob's servers. This is recommended only for users who *require* SSL encryption
on their HTML5 applications, and you must take care to ensure that your HTML5 application will still function as desired
when under encryption.

**Important Note:** since your website will now be encrypted, any outside resources that your website tries to access
that are not themselves sent over an encrypted connection will cause a warning to appear in most web browsers. This
applies to resources such as JavaScript modules, CSS files, images, video, and so on. If you choose to use SSL for your
HTML5 application, we recommend that you double-check the documentation of any JavaScript modules you may be using for
caveats involving HTTPS, and that you verify HTTPS for as many resources and modules as you can.

### Experience Level

Intermediate

### Estimated time to complete

~20 minutes

### Prerequisites

* A registered domain name for your application (here, `www.example.com`)
* Active [HTML5](https://dashboard.stackmob.com/module/html5), [Custom Domains](https://dashboard.stackmob.com/module/customdomains), and [SSL](https://dashboard.stackmob.com/module/ssl) modules for your application
* [OpenSSL](http://www.openssl.org/), to generate a key and certificate signing request

# Let's get started!

### Choosing a Certificate Authority

Before we can work with SSL certificates, it is up to you to choose who will be your [Certificate Authority](http://en.wikipedia.org/wiki/Certificate_authority), or CA. There are several companies that offer CA services, including:

* [Symantec](http://www.symantec.com/ssl-certificates), formerly VeriSign
* [GoDaddy](http://www.godaddy.com/ssl/), and many other domain name providers
* [Comodo](http://www.comodo.com/)
* [GeoTrust](http://www.geotrust.com/)

Additionally, you can generate a [self-signed certificate](http://en.wikipedia.org/wiki/Self-signed_certificate), which can be useful in testing development applications, but which are not suitable for production. We will not discuss self-signed certificates here.

We cannot recommend one CA over another, and it is up to you to research which CA fits your needs. You only need a certificate for your domain, e.g. `www.example.com`, you do not need a wildcard certificate.

### Creating your Private Key and Signing Key

First, we have to acquire the correct SSL credentials: a [certificate](http://en.wikipedia.org/wiki/Public_key_certificate) for your domain, an [intermediate certificate](http://en.wikipedia.org/wiki/Intermediate_certificate_authorities) bundle, and a [private key](http://en.wikipedia.org/wiki/Private_key) that will be used to sign the domain certificate. To acquire the certificates, we must first generate a private key.

Some CA websites will offer to generate this for you, but most (if they require a private key) will let you use your own. To use SSL with StackMob, you *must* supply the signing key that is tied to your certificate, so if your CA offers to generate the key, be sure that you can get the key back from them!

To generate a private key, we will use OpenSSL. To verify that you have OpenSSL installed correctly, issue the following command in your terminal:

    which openssl
    
You should receive back `/usr/bin/openssl` or similar. If not, you will need to install OpenSSL.

* In OS X, this can be done through [Homebrew](http://mxcl.github.com/homebrew/) (`brew install openssl`).
* In Linux, OpenSSL should be an available package through your package manager (`apt-get install openssl` in Ubuntu/Debian, `yum install openssl` in RHEL/CentOS, etc.)
* In Windows, [download and install a binary package](http://gnuwin32.sourceforge.net/packages/openssl.htm).

Once you have OpenSSL ready, we can create a private key. Issue the following command in your terminal:

    openssl genrsa -des3 -out myprivate.key 2048

This will prompt you for a pass phrase. Be sure to use a strong password here, and to remember this password! When the program finishes, you will have a file called `myprivate.key`. Keep this file!

Next, you will need to create a signing key from your private key, which is what will actually be used in the next step. Issue the following command in your terminal:

    openssl rsa -in myprivate.key -out mysigning.key

Now you will also have a file called `mysigning.key`. Keep this file, too!

**Important Note:** The signing key is actually a passphrase-less version of your private key, so be sure to keep it secure! We do **not** store your key on our servers for exactly this reason. Once we give it to our hosting provider to configure your SSL settings (which itself is done over a secure connection), it's gone and we can't get it back!

### Creating your Certificate Signing Request

Next, to request a new certificate from a CA, you will most likely need to send the CA a [Certificate Signing Request](http://en.wikipedia.org/wiki/Certificate_signing_request), or CSR.

These steps vary between CAs, and we cannot offer support for this as a result, so please refer to your CA's documentation on how to generate a CSR. For example, [Symantec's documentation](https://knowledge.verisign.com/support/ssl-certificates-support/index?page=content&id=AR198) requires that you do not enter an email address, and [GoDaddy's documentation](http://support.godaddy.com/help/article/5269/generating-a-certificate-signing-request-csr-apache-2x) has you generate a key as part of their instructions in generating the CSR. The most important part is that you preserve the key used to sign the CSR, because that key becomes tied to the certificate you receive.

Generally, to generate a CSR, issue the following command in your terminal:

    openssl req -new -key mysigning.key -out myserver.csr

You will be asked for a few pieces of information, including:

* Country Name, e.g. `US`, which corresponds to an [ISO 3166-1](http://en.wikipedia.org/wiki/ISO_3166-1_alpha-2) country code
* State or Province Name, e.g. `California`
* Locality Name, e.g. `San Francisco`
* Organization Name, e.g. `StackMob, Inc.` Special characters like ampersands and at-symbols should be omitted or spelled out.
* Organizational Unit, which can be omitted
* Common Name, e.g. `www.example.com`
* Email Address, which some companies require to be omitted
* Challenge Password, which should be omitted
* Optional Company Name, which should be omitted

Again, refer to your CA's documentation for their specific guidelines about how to create a CSR. When you are finished, you will have a CSR called `myserver.csr`, which you can send to your CA.

### Retrieving your Certificates

After sending off your CSR to your CA, you should receive either a zipped archive containing two certificates, or just two certificates. One of these certificates will be the certificate for your domain, and the other will be an intermediate certificate. Again, these steps vary between CAs, but it is most important to retrieve both certificate files.

Both files should end in `.crt`, and can be viewed in a text editor. Each should begin with `-----BEGIN CERTIFICATE-----`, and be followed by a large block of encoded text, then a closing tag. The signing key will also look similar.

About the intermediate certificate: not every CA service will return certificates in the proper order. Please verify that your chain of certificates matches that of walking up the chain from the domain to the CA root. Your CA is the only organization that can help in verifying this, so please contact them directly if you have problems.

### Uploading your Certificates and Key to StackMob

**Important Note:** if you have previously been using a custom domain in StackMob that you are now **upgrading** to an SSL-enabled custom domain, **you must first delete the existing custom domain entry**. You can do that in the [Custom Domain module](https://dashboard.stackmob.com/module/customdomains) Settings for your application. You will be prompted to do so as part of the creation step if you have not already.

Finally, open the [SSL module](https://dashboard.stackmob.com/module/ssl) on the StackMob Dashboard, and click Settings. 

Here, you will see a text field for your soon-to-be secure hostname. Enter your hostname here, e.g. `www.example.com`.

You will also see text areas for the domain certificate, intermediate certificate, and signing key. Drag-and-drop each file into these text areas. You may alternatively copy-and-paste the contents of these files, but be sure to use "Select All" when copying because the entire file is important (including line breaks).

You also need to specify whether this is to be a development or production hostname. Most likely, you will be securing your production environment, and not your development environment.

When you are ready, click Upload, and the process will begin on our side.

### Setting the CNAME Record

If everything was successfully accepted, you will be presented with a StackMob internal hostname that is specific to the hostname you entered, e.g. for `www.example.com` it will look like `wwwexamplecom.cname.stackmobapp.com`. This internal hostname is an A record, or host record. You will now need to set up a CNAME (Alias) record for your domain name that points to this internal hostname. From a technical standpoint, the chain of hostnames will look something like:

    www.example.com -> wwwexamplecom.cname.stackmobapp.com -> our hosting provider -> StackMob

Setting up a CNAME record is done through your domain name provider, and the steps also vary by domain name provider, so please refer to your provider's documentation on this. For example, [GoDaddy's documentation](http://support.godaddy.com/help/article/7921/adding-or-editing-cname-records) has step-by-step instructions, and [NameCheap's documentation](http://www.namecheap.com/support/knowledgebase/article.aspx/1031/2/demo-how-to-set-up-a-cname-record) features a demo video on the subject.

At this point, you're finished! As this is reliant on several technologies, it may take a while before the new settings have fully propagated. This is normal, and is a result of the decentralized nature of the domain name system. **If your hostname, certificates, and key were successfully accepted by us, please allow up to 24 hours before contacting us for support.**

Once all changes have settled, you should be able to access your domain over HTTPS, e.g. `https://www.example.com`. Congratulations!