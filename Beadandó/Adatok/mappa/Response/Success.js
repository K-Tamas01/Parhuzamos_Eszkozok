const { EmbedBuilder } = require('discord.js');

module.exports = {
	run:async (msg, code, guildqueue, args, user) => {
		switch (code) {
		case 0:{
			const content = new EmbedBuilder()
				.setTitle(`Most játszom éppen: ${guildqueue.nowPlaying.name}`)
				.setColor('#FF1493')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		case 1:{

			let min_part = 1;
			const piece = parseInt(guildqueue.songs.length / 25);
			let max_part, iteration = 0;
			if (piece === 0) {
				max_part = guildqueue.songs.length;
			}
			else {
				max_part = parseInt(guildqueue.songs.length / piece);
			}

			do {
				let content;
				if (user === undefined) {
					content = new EmbedBuilder()
						.setTitle('Lejátszási listám:')
						.setColor('#FF1493')
						.setFooter({ text:`Requested by ${msg.author.username}` })
						.setTimestamp();

					for (let i = min_part; i < max_part; i++) {
						content.addFields({ name: guildqueue.songs[i].name, value: ' [' + guildqueue.songs[i].duration + ']', inline: false });
					}
				}
				else {
					content = new EmbedBuilder()
						.setTitle('Lejátszási listám:')
						.setColor('#FF1493')
						.setFooter({ text:`Requested by ${user}` })
						.setTimestamp();

					for (let i = min_part; i < max_part; i++) {
						content.addFields({ name: guildqueue.songs[i].name, value: ' [' + guildqueue.songs[i].duration + ']', inline: false });
					}
				}
				msg.channel.send({ embeds: [content] });

				min_part = max_part;
				if ((max_part + max_part) >= guildqueue.songs.length) {
					max_part = guildqueue.songs.length;
				}
				else {
					max_part += max_part;
					iteration++;
				}
			}
			while (iteration < piece);
			break;
		}
		case 2:{
			const content = new EmbedBuilder()
				.setTitle(`Az alábbi szám sikeresen törölve a listáról: ${guildqueue.songs[args].name}`)
				.setColor('#FF1493')
				.setFooter({ text:`Requested by ${msg.author.username}` })
				.setTimestamp();

			msg.channel.send({ embeds: [content] });
			break;
		}
		}
	},
};