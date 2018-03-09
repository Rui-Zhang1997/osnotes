create database if not exists ctx;
use ctx;
create table article_meta(
    author varchar(255),
    title varchar(255),
    description varchar(255),
    url varchar(255),
    urlToImage varchar(255),
    publishedAt datetime,
    sid varchar(50),
    id integer not null auto_increment,
    primary key (id),
    foreign key (sid) references source(sid)
);

create table source(
    sid varchar(50),
    name varchar(100),
    primary key (sid)
);
