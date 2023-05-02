require('dotenv').config();
const Discord = require('discord.js');
const bot = new Discord.Client({
	intents: [
		Discord.IntentsBitField.Flags.Guilds,
		Discord.IntentsBitField.Flags.GuildMembers,
		Discord.IntentsBitField.Flags.GuildMessages,
		Discord.IntentsBitField.Flags.MessageContent,
		Discord.IntentsBitField.Flags.GuildVoiceStates,
	],
});
const { cookie, prefix } = require('./Config/Conf.json');
const { Player } = require('discord-music-player');
const player = new Player(bot, {
	leaveOnEmpty: false,
	ytdlRequestOptions: { Headers: { Cookie:cookie } },
});
const fs = require('fs');
const radio = require('./Response/Radio-button');

bot.player = player;
bot.command = new Discord.Collection();
bot.aliases = new Discord.Collection();
bot.categ = fs.readdirSync('./Handler/Commands');

['Handler'].forEach(handler => {
	require(`./Handler/${handler}`)(bot);
});

let guildqueue;
let guildmsg;
let vol = 100;

bot.on('messageCreate', async (msg) => {

	guildqueue = bot.player.getQueue(msg.guild.id);
	guildmsg = msg;

	if (msg.author.bot) return;
	if (!msg.content.startsWith(prefix)) return;

	const args = msg.content.slice(prefix.length).trim().split(/ +/g);
	let cmd = args.shift().toLowerCase();

	if (cmd === 0) cmd = 'help';

	let command = bot.command.get(cmd);

	if (!command) command = bot.command.get(bot.aliases.get(cmd));

	if (command === undefined) {
		return;
	}

	command.run(bot, msg, args, guildqueue);

});

bot.player.on('songFirst', (_guildqueue, song) => {
	radio.run(guildmsg, song, undefined);
});

bot.player.on('songChanged', (_guildqueue, newsong) => {
	radio.run(guildmsg, undefined, newsong);
});

bot.player.on('queueEnd', () => {
	radio.run(guildmsg, undefined, undefined);
});

bot.player.on('queueDestroyed', () => {
	radio.run(guildmsg, undefined, undefined);
});

bot.on('interactionCreate', async (interact) => {
	if (!interact.isButton()) return;

	if (interact.member.voice.channel === null || interact.guild.members.me.voice.channel === null || interact.member.voice.channel.id !== interact.guild.members.me.voice.channel.id) {
		interact.deferUpdate();
		return;
	}

	let cmd;

	switch (interact.customId) {
	case 'Play':{
		cmd = 'continue';
		break;
	}
	case 'volume+':{
		cmd = 'volume';
		if ((vol + 10) > 100) break;
		else vol += 10;
		break;
	}
	case 'volume-':{
		cmd = 'volume';
		if ((vol - 10) < 0) break;
		else vol -= 10;
		break;
	}
	case 'Skip':{
		cmd = 'skip';
		break;
	}
	case 'Leave':{
		cmd = 'leave';
		break;
	}
	case 'Pause':{
		cmd = 'pause';
		break;
	}
	case 'Queue':{
		cmd = 'queue';
		break;
	}
	}

	let command = bot.command.get(cmd);

	if (!command) command = bot.command.get(bot.aliases.get(cmd));

	switch (interact.customId) {
	case 'Play':{
		command.run(bot, guildmsg, [], guildqueue, undefined);
		interact.deferUpdate();
		break;
	}
	case 'volume+':{
		command.run(guildqueue, vol);
		interact.deferUpdate();
		break;
	}
	case 'volume-':{
		command.run(guildqueue, vol);
		interact.deferUpdate();
		break;
	}
	case 'Skip':{
		command.run(bot, guildmsg, [], guildqueue, undefined);
		interact.deferUpdate();
		break;
	}
	case 'Leave':{
		command.run(bot, guildmsg, [], guildqueue, undefined);
		interact.deferUpdate();
		break;
	}
	case 'Pause':{
		command.run(bot, guildmsg, [], guildqueue, undefined);
		interact.deferUpdate();
		break;
	}
	case 'Queue':{
		command.run(bot, guildmsg, [], guildqueue, interact.user.username);
		interact.deferUpdate();
		break;
	}
	}
});

bot.on('ready', () => {
	console.log('Online');

	const stat = [
		'Prefix: ' + prefix,
		prefix + 'help',
		'Üzemen kivül!',
	];

	setInterval(function() {
		const status = stat[Math.floor(Math.random() * stat.length)];
		bot.user.setPresence({
			activities: [{ name: status, type: Discord.ActivityType.Watching }],
			status: 'online',
		});
	}, 5000);
});

bot.login(process.env.TOKEN);