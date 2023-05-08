const error = require('../../../Response/Error');
const { isplaylist } = require('../../../Root_Service/Service');

module.exports = {
	name:'play',
	category:'Service',
	description:'$play url/cím',
	isload: true,
	run: async (bot, msg, args, guildqueue) => {
		if (args.length !== 0) {
			if (msg.member.voice.channel !== null) {
				if (msg.guild.members.me.voice.channel === null || msg.guild.members.me.voice.channel.id === msg.member.voice.channel.id) {
					if (isplaylist(args)) {
						try {
							const queue = bot.player.createQueue(msg.guild.id);
							await queue.join(msg.member.voice.channel);
							await queue.play(args.join(' ')).catch((err) => {
								if (!guildqueue) {
									error.run(msg, -1, err.message);
									queue.stop();
								}
							});
						}
						catch (err) {
							console.log(err);
							error.run(msg, -1, err);
						}
					}
					else {
						try {
							const queue = bot.player.createQueue(msg.guild.id);
							await queue.join(msg.member.voice.channel);
							await queue.playlist(args.join(' ')).catch((err) => {
								console.log(err);
								if (!guildqueue) {
									error.run(msg, -1, err.message);
									queue.stop();
								}
							});
						}
						catch (err) {
							console.log(err);
							error.run(msg, -1, err);
						}
					}
				}
				else {
					error.run(msg, 3);
				}
			}
			else {
				error.run(msg, 2);
			}
		}
		else {
			error.run(msg, 5);
		}
	},
};